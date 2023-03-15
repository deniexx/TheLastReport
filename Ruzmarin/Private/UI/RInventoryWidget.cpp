


#include "UI/RInventoryWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RInventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RInventorySlot.h"

void URInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseDropdownImage->OnMouseButtonDownEvent.BindDynamic(this, &URInventoryWidget::CloseDropdownMenu);
	CurrentSlotSelected = -1;

	APawn* MyPawn = GetOwningPlayerPawn();

	if (!ensure(MyPawn)) return;
	
	URInventoryComponent* Inventory = URInventoryComponent::GetInventoryComponent(MyPawn);

	if (ensure(Inventory) && ensure(InventorySlotClass))
	{
		const uint8 FirstLoopMax = (Inventory->AvailableInventorySlots / Inventory->InventorySlotsPerRow);
		const uint8 SecondLoopMax = Inventory->InventorySlotsPerRow;
		uint8 SlotIndex = 0;

		for (int i = 0; i < FirstLoopMax; ++i)
		{
			for (int j = 0; j < SecondLoopMax; ++j)
			{
				URInventorySlot* InventorySlot = CreateWidget<URInventorySlot>(this, InventorySlotClass);
				InventorySlot->SlotIndex = SlotIndex;
				InventorySlot->OnInventorySlotClicked.AddDynamic(this, &URInventoryWidget::OnSlotClicked);
				InventoryUniformGrid->AddChildToUniformGrid(InventorySlot, i, j);
				InventorySlots.Add(InventorySlot);
				++SlotIndex;
			}
		}

		Inventory->OnInventoryItemAdded.AddDynamic(this, &URInventoryWidget::OnInventoryItemAdded);
		Inventory->OnInventoryItemUsed.AddDynamic(this, &URInventoryWidget::OnInventoryItemUsed);

		DropDownMenu->UseItemButton->OnReleased.AddDynamic(this, &URInventoryWidget::OnUseItemClicked);
		DropDownMenu->DropItemButton->OnReleased.AddDynamic(this, &URInventoryWidget::OnDropItemClicked);
	}
}

void URInventoryWidget::OnInventoryItemAdded(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex)
{
	InventorySlots[SlotIndex]->UpdateSlot(ItemTexture, ItemAmount);
}

void URInventoryWidget::OnInventoryItemUsed(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex)
{
	if (ItemAmount > 0)
		InventorySlots[SlotIndex]->UpdateSlot(ItemTexture, ItemAmount);

	else
		InventorySlots[SlotIndex]->UpdateSlot(EmptySlotTexture, ItemAmount);
}

void URInventoryWidget::OnSlotClicked(uint8 SlotIndex)
{
	if (SlotIndex == CurrentSlotSelected) return;
	
	const URInventoryComponent* Inventory = URInventoryComponent::GetInventoryComponent(GetOwningPlayerPawn());
	if (!Inventory) return;

	const FGameplayTag ItemTag = Inventory->GetItemTagAtSlot(SlotIndex);
	if (ItemTag == Inventory->EmptyItemSlotTag) return;

	const ARGameMode* GameMode = GetWorld()->GetAuthGameMode<ARGameMode>();
	if (!GameMode) return;

	const FInventoryItemInfoRow InfoRow = GameMode->GetItemInfoWithTag(ItemTag);

	CurrentSlotSelected = SlotIndex;
	ItemName->SetText(InfoRow.ItemName);
	ItemDescription->SetText(InfoRow.ItemDescription);
	CloseDropdownImage->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenItemText);
	
	const FGeometry Geometry = InventorySlots[SlotIndex]->GetCachedGeometry();
	const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	
	FVector2D NewTranslation = Geometry.GetAbsolutePosition() / Scale;
	NewTranslation.X += XDropDownOffset;
	NewTranslation.Y += YDropDownOffset;
	
	DropDownMenu->SetRenderTranslation(NewTranslation);
	DropDownMenu->PlayAnimation(DropDownMenu->OpenDropdown);
}

void URInventoryWidget::OnUseItemClicked()
{
	const APawn* MyPawn = GetOwningPlayerPawn();
	if (!MyPawn) return;
	
	URInventoryComponent* Inventory = URInventoryComponent::GetInventoryComponent(GetOwningPlayerPawn());
	if (Inventory)
	{
		Inventory->UseInventoryItem(CurrentSlotSelected);
	}
}

void URInventoryWidget::OnDropItemClicked()
{
	const APawn* MyPawn = GetOwningPlayerPawn();
	if (!MyPawn) return;
	
	URInventoryComponent* Inventory = URInventoryComponent::GetInventoryComponent(GetOwningPlayerPawn());
	if (Inventory)
	{
		Inventory->DropInventoryItem(CurrentSlotSelected);
	}
}

FEventReply URInventoryWidget::CloseDropdownMenu(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply EventReply;
	EventReply.NativeReply = FReply::Handled();

	HideDropDownMenu();
	
	return EventReply;
}

void URInventoryWidget::HideDropDownMenu()
{
	CurrentSlotSelected = -1;
	
	PlayAnimationReverse(OpenItemText);
	DropDownMenu->PlayAnimationReverse(DropDownMenu->OpenDropdown);

	CloseDropdownImage->SetVisibility(ESlateVisibility::Collapsed);
}
