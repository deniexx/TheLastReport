


#include "Components/RInventoryComponent.h"

#include "Actor/RInventoryInteractableActor.h"
#include "Actors/RInventoryPickUpActor.h"
#include "Components/RActionComponent.h"
#include "Data/RInventoryItemData.h"
#include "Engine/AssetManager.h"
#include "GameFramework/RGameMode.h"
#include "GameFramework/RPlayerController.h"
#include "Interfaces/RGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Ruzmarin/Ruzmarin.h"
#include "UI/RInventoryWidget.h"

// Sets default values for this component's properties
URInventoryComponent::URInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActorEyesOffset = 44;
	DropRange = 250.0f;
}

void URInventoryComponent::SetUp()
{
	for (uint8 i = 0; i < AvailableInventorySlots; ++i)
	{
		FInventoryItem InventoryItem;
		InventoryItem.ItemTag = EmptyItemSlotTag;
		InventoryItem.SlotIndex = i;
		InventoryItem.Amount = 0;
		InventoryItems.Add(InventoryItem);
	}

	if (InventoryWidgetInstance == nullptr && ensure(InventoryWidgetClass))
	{
		ARCharacter* MyOwner = Cast<ARCharacter>(GetOwner());

		if (MyOwner)
		{
			CharacterOwner = MyOwner;
			APlayerController* PC = MyOwner->GetCurrentPC();
			InventoryWidgetInstance = CreateWidget<URInventoryWidget>(PC, InventoryWidgetClass);
		
			InventoryWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
			InventoryWidgetInstance->AddToViewport();
		}
	}

	GetWorld()->GetAuthGameMode<ARGameMode>()->SetUpInventoryBindings(this);
}

URInventoryComponent* URInventoryComponent::GetInventoryComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URInventoryComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

FGameplayTag URInventoryComponent::GetItemTagAtSlot(uint8 SlotIndex) const
{
	return InventoryItems[SlotIndex].ItemTag;
}

bool URInventoryComponent::IsInventoryVisible() const
{
	if (!InventoryWidgetInstance) return false;
	
	return InventoryWidgetInstance->GetVisibility() == ESlateVisibility::Visible;
}

void URInventoryComponent::ToggleInventory(AActor* Instigator)
{
	if (!InventoryWidgetInstance) return;

	InstigatorActor = Instigator;
	
	const bool bVisible = InventoryWidgetInstance->GetVisibility() == ESlateVisibility::Visible;
	ARPlayerController* PC = CharacterOwner->GetController<ARPlayerController>();
	if (!PC) return;

	if (bVisible)
	{
		InventoryWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		InventoryWidgetInstance->HideDropDownMenu();
		PC->SetInputMode(FInputModeGameOnly());
		PC->ResetIgnoreInputFlags();
		PC->bShowMouseCursor = false;
		PC->ToggleHUD(EHUDVisibilityOption::AllVisible);
	}
	
	else
	{
		if (PC->GetIsPlayerInAnyUI()) return;
		
		InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
		PC->bShowMouseCursor = true;
		PC->ToggleHUD(EHUDVisibilityOption::OnlyAttributeBar);
	}

	PC->SetIsInAnyUI(!bVisible);
}

bool URInventoryComponent::AddItemToInventory(FGameplayTag ItemToAdd)
{

	const ARGameMode* GameMode = GetWorld()->GetAuthGameMode<ARGameMode>();
	if (!ensure(GameMode)) return false;

	const FInventoryItemInfoRow ItemInfo = GameMode->GetItemInfoWithTag(ItemToAdd);

	if (!CheckCanItemBeAddedToInventory(ItemToAdd, ItemInfo.MaxStackAmount)) return false;
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &URInventoryComponent::OnAddInventoryItemLoaded, ItemInfo.ItemID, ItemInfo);

		LogOnScreen(this, FString::Printf(TEXT("Loading Add Item")), FColor::Green);
		UE_LOG(LogTemp, Log, TEXT("Loading Add Item"));
		Manager->LoadPrimaryAsset(ItemInfo.ItemID, Bundles, Delegate);
		
		return true;
	}
	
	return false;
}

void URInventoryComponent::OnAddInventoryItemLoaded(FPrimaryAssetId LoadedID, FInventoryItemInfoRow InfoRow)
{
	UE_LOG(LogTemp, Log, TEXT("Item Add Loaded"));

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		URInventoryItemData* ItemData = Cast<URInventoryItemData>(Manager->GetPrimaryAssetObject(LoadedID));

		if (ItemData)
		{
			const uint8 BestSlot = FindBestSlotForItemWithTag(InfoRow.ItemTag, InfoRow.MaxStackAmount);
			ensureAlways(BestSlot != -1);

			const uint8 NewAmount = InventoryItems[BestSlot].Amount + 1;
			
			InventoryItems[BestSlot].ItemTag = InfoRow.ItemTag;
			InventoryItems[BestSlot].Amount = NewAmount;
			
			OnInventoryItemAdded.Broadcast(InfoRow.ItemTag, ItemData->ItemIcon, NewAmount, BestSlot);
		}
	}
}

bool URInventoryComponent::UseInventoryItem(uint8 ItemSlot)
{
	if (InventoryItems[ItemSlot].ItemTag == EmptyItemSlotTag || InventoryItems[ItemSlot].Amount <= 0) return false;
	
	const ARGameMode* GameMode = GetWorld()->GetAuthGameMode<ARGameMode>();
	if (!ensure(GameMode)) return false;

	const FInventoryItemInfoRow ItemInfo = GameMode->GetItemInfoWithTag(InventoryItems[ItemSlot].ItemTag);

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &URInventoryComponent::OnUseItemLoaded, ItemInfo.ItemID, ItemInfo, ItemSlot);

		LogOnScreen(this, FString::Printf(TEXT("Loading Use Item")), FColor::Green);
		UE_LOG(LogTemp, Log, TEXT("Loading Use Item"));
		Manager->LoadPrimaryAsset(ItemInfo.ItemID, Bundles, Delegate);
		
		return true;
	}
	
	return false;
}

void URInventoryComponent::OnUseItemLoaded(FPrimaryAssetId LoadedID, FInventoryItemInfoRow InfoRow, uint8 ItemSlot)
{
	UE_LOG(LogTemp, Log, TEXT("Item Use Loaded"));
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		bool bChangeHasHappened = false;
		bool bShouldCloseInventory = false;
		
		URInventoryItemData* ItemData = Cast<URInventoryItemData>(Manager->GetPrimaryAssetObject(LoadedID));

		if (InstigatorActor->Implements<URGameplayInterface>())
		{
			const ARInventoryInteractableActor* InteractableActor = Cast<ARInventoryInteractableActor>(InstigatorActor);

			// Return if tags are not the same, so we do not do anything on the items
			if (!InteractableActor || !InteractableActor->AcceptedGameplayTags.HasTagExact(InventoryItems[ItemSlot].ItemTag)) return;
			
			APawn* MyPawn = Cast<APawn>(GetOwner());
			if (MyPawn)
			{
				IRGameplayInterface::Execute_AfterInventoryItemUsed(InstigatorActor, MyPawn, InfoRow.ItemTag);
				bChangeHasHappened = true;
				bShouldCloseInventory = true;
			}
		}
		
		if (ItemData && ItemData->ItemAction)
		{
			URActionComponent* ActionComponent = URActionComponent::GetActionComponent(CharacterOwner);
			if (ActionComponent)
			{
				if (ItemData->ItemAction)
				{
					ActionComponent->AddAction(CharacterOwner, ItemData->ItemAction);
					if (!ActionComponent->StartActionByTag(CharacterOwner, InfoRow.ItemTag))
					{
						ActionComponent->RemoveAction(InfoRow.ItemTag);
						return;
					}
					bChangeHasHappened = true;
				}
			}
		}
		
		if (!bChangeHasHappened) return;

		const uint8 NewAmount = FMath::Clamp<uint8>(InventoryItems[ItemSlot].Amount - 1, 0, InfoRow.MaxStackAmount);
		InventoryItems[ItemSlot].Amount = NewAmount;

		if (NewAmount == 0)
		{
			InventoryItems[ItemSlot].ItemTag = EmptyItemSlotTag;
		}
		
		OnInventoryItemUsed.Broadcast(InventoryItems[ItemSlot].ItemTag, ItemData->ItemIcon, NewAmount, ItemSlot);
		InventoryWidgetInstance->HideDropDownMenu();

		if (bShouldCloseInventory) ToggleInventory(nullptr);
	}
}

bool URInventoryComponent::DropInventoryItem(uint8 ItemSlot)
{
	if (InventoryItems[ItemSlot].ItemTag == EmptyItemSlotTag || InventoryItems[ItemSlot].Amount <= 0) return false;

	const ARGameMode* GameMode = GetWorld()->GetAuthGameMode<ARGameMode>();
	if (!ensure(GameMode)) return false;

	FInventoryItemInfoRow ItemInfo = GameMode->GetItemInfoWithTag(InventoryItems[ItemSlot].ItemTag);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &URInventoryComponent::OnDropItemLoaded, ItemInfo.ItemID, ItemInfo, ItemSlot);

		LogOnScreen(this, FString::Printf(TEXT("Loading Drop Item")), FColor::Green);
		UE_LOG(LogTemp, Log, TEXT("Loading Drop Item"));
		Manager->LoadPrimaryAsset(ItemInfo.ItemID, Bundles, Delegate);
		
		return true;
	}
	
	return false;
}

void URInventoryComponent::OnDropItemLoaded(FPrimaryAssetId LoadedID, FInventoryItemInfoRow InfoRow, uint8 ItemSlot)
{
	UE_LOG(LogTemp, Log, TEXT("Item Drop Loaded"));

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		URInventoryItemData* ItemData = Cast<URInventoryItemData>(Manager->GetPrimaryAssetObject(LoadedID));
		if (!ItemData) return;
		
		FHitResult Hit;

		FVector TraceStart = GetOwner()->GetActorLocation();
		TraceStart.Z += ActorEyesOffset;
		
		FVector TraceEnd = TraceStart + (GetOwner()->GetActorForwardVector() * DropRange);
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility);

		FVector Location = bHit ? Hit.ImpactPoint : TraceEnd;

		/* Load the Static Mesh of the item */
		UStaticMesh* StaticMesh = ItemData->ItemMesh.LoadSynchronous();

		/* Spawn Actor */
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ARInventoryPickUpActor* Actor = GetWorld()->SpawnActor<ARInventoryPickUpActor>(InventoryPickUpActorClass, Location, FRotator::ZeroRotator, SpawnParams);

		/* Apply Tag and Static Mesh */
		Actor->ItemTag = InventoryItems[ItemSlot].ItemTag;
		Actor->SetMesh(StaticMesh);
		Actor->SetPhysicsOn();
		Actor->SetScaleAndSphereRadius(ItemData->DefaultItemScale, ItemData->BoxScale, ItemData->BoxLocation);

		/* Update Inventory UI */
		const uint8 NewAmount = FMath::Clamp<uint8>(InventoryItems[ItemSlot].Amount - 1, 0, InfoRow.MaxStackAmount);
		InventoryItems[ItemSlot].Amount = NewAmount;

		if (NewAmount == 0)
		{
			InventoryItems[ItemSlot].ItemTag = EmptyItemSlotTag;
		}
		
		OnInventoryItemUsed.Broadcast(InventoryItems[ItemSlot].ItemTag, ItemData->ItemIcon, NewAmount, ItemSlot);
		InventoryWidgetInstance->HideDropDownMenu();
	}
}

uint8 URInventoryComponent::FindBestSlotForItemWithTag(const FGameplayTag& ItemTag, const uint8 MaxStackAmount)
{
	for (const FInventoryItem& InventoryItem : InventoryItems)
	{
		if (InventoryItem.ItemTag.MatchesTagExact(ItemTag))
		{
			if (InventoryItem.Amount < MaxStackAmount)
			{
				return InventoryItem.SlotIndex;
			}
		}
	}
	
	/* If above loop does not find another item that we can add amount to, we search for an empty slot */
	for (const FInventoryItem& InventoryItem : InventoryItems)
	{
		if (InventoryItem.ItemTag.MatchesTagExact(EmptyItemSlotTag))
		{
			return InventoryItem.SlotIndex;
		}
	}

	// If slot is not found return -1
	return -1;
}

bool URInventoryComponent::CheckCanItemBeAddedToInventory(const FGameplayTag& ItemTag, const uint8 MaxStackAmount)
{
	for(const FInventoryItem& InventoryItem : InventoryItems)
	{
		if (InventoryItem.ItemTag.MatchesTagExact(EmptyItemSlotTag))
			return true;
		if (InventoryItem.Amount + 1 <= MaxStackAmount)
			return true;
	}

	return false;
}
