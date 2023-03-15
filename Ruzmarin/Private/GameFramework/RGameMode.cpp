


#include "GameFramework/RGameMode.h"

#include "EngineUtils.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Actors/RInventoryPickUpActor.h"
#include "Actors/RRandomLocationActor.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Components/RInventoryComponent.h"
#include "GameFramework/RPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "UI/LoadingScreenWidget.h"


void ARGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::LoadStreamLevel(this, "MainMenu", true, false, FLatentActionInfo());
}

void ARGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ARGameMode::SetUpInventoryBindings(URInventoryComponent* InventoryComponent)
{
	InventoryComponent->OnInventoryItemAdded.AddDynamic(this, &ARGameMode::ExecuteSpecialEventOnInventoryItemAdded);
	InventoryComponent->OnInventoryItemUsed.AddDynamic(this, &ARGameMode::ExecuteSpecialEventOnInventoryItemUsed);
}

void ARGameMode::RandomizeItems()
{
	// Get all Random location actors and store them in an array
	TArray<ARRandomLocationActor*> RandomLocationActors;
	for (TActorIterator<ARRandomLocationActor> It(GetWorld()); It; ++It)
	{
		RandomLocationActors.Add(*It);
	}
	
	if (RandomLocationActors.Num() > 0)
	{
		// Move/Spawn actors at the appropriate location
		for (TActorIterator<ARInventoryPickUpActor> It(GetWorld()); It; ++It)
		{
			SpawnActorAtBestLocation(*It, &RandomLocationActors);
			if (RandomLocationActors.Num() == 0) break;
		}
	}
}

void ARGameMode::SpawnActorAtBestLocation(ARInventoryPickUpActor* ActorToSpawn, TArray<ARRandomLocationActor*>* RandomLocationActors)
{
	if (!ActorToSpawn->bRandomizeLocation) return;
	if (RandomLocationActors->Num() == 0) return;

	const int32 Index = FMath::RandRange(0, RandomLocationActors->Num() - 1);
	ARRandomLocationActor* RandomLocationActor = (*RandomLocationActors)[Index];
	
	if (ActorToSpawn->ItemSize <= RandomLocationActor->ItemSize)
	{
		if (ActorToSpawn->AllowedSpawnLocations.HasTagExact(RandomLocationActor->CurrentSpawnLocation))
		{
			AActor* ParentActor = RandomLocationActor->GetAttachParentActor();
			ActorToSpawn->SetActorLocation(RandomLocationActor->GetActorLocation());
			
			const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
			ActorToSpawn->AttachToActor(ParentActor, AttachmentTransformRules);
			RandomLocationActor->Destroy();
			RandomLocationActors->RemoveAt(Index);
			return;
		}
	}
	
	SpawnActorAtBestLocation(ActorToSpawn, RandomLocationActors);
}

void ARGameMode::PlayGame()
{
	LoadingWidgetInstance = CreateWidget<ULoadingScreenWidget>(UGameplayStatics::GetPlayerController(this, 0), LoadingWidgetClass);
	LoadingWidgetInstance->AddToViewport();

	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), Cameras);
		
	TUniqueFunction<void()> Task = [this, Cameras]
	{
		UGameplayStatics::UnloadStreamLevel(this, "MainMenu", FLatentActionInfo(), false);
		
		FLatentActionInfo LatentActionInfo;
		LatentActionInfo.CallbackTarget = this;
		LatentActionInfo.ExecutionFunction = "GetReadyForPlay";
		LatentActionInfo.UUID = 123;
		LatentActionInfo.Linkage = 0;
		UGameplayStatics::LoadStreamLevel(this, "PlayLevel", true, false, LatentActionInfo);
		for (AActor* Camera : Cameras)
		{
			Camera->Destroy();
		}
	};
	
	Async(EAsyncExecution::TaskGraph,MoveTemp(Task));
}

void ARGameMode::GetReadyForPlay()
{
	PlayerControllerClass = CustomPlayerControllerClass;
	DefaultPawnClass = CustomDefaultPawnClass;

	APlayerController* MyPC = SpawnPlayerControllerCommon(ROLE_Authority, FVector::ZeroVector, FRotator::ZeroRotator, CustomPlayerControllerClass);

	SwapPlayerControllers(UGameplayStatics::GetPlayerController(this, 0), MyPC);
	
	APawn* MyPawn = SpawnDefaultPawnAtTransform(MyPC, PlayerSpawnTransform);
	
	RandomizeItems();
	MyPC->Possess(MyPawn);
	MyPawn->PossessedBy(MyPC);

	ARPlayerController* RPC = Cast<ARPlayerController>(MyPC);
	RPC->AfterLoad();

	ARCharacter* RC = Cast<ARCharacter>(MyPawn);
	RC->OnPosses();
	
	LoadingWidgetInstance->ShowTextBlock();

	RPC->StopAll(true);
}

void ARGameMode::CloseLoadingScreenWidget()
{
	if (LoadingWidgetInstance)
	{
		LoadingWidgetInstance->RemoveFromParent();
		LoadingWidgetInstance = nullptr;
		ARPlayerController* PC = Cast<ARPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		PC->StopAll(false);
	}
}

/* Get Item Data From Data Table */
FInventoryItemInfoRow ARGameMode::GetItemInfoWithTag(const FGameplayTag ItemTag) const
{
	if (!ensure(ItemDataTable)) return FInventoryItemInfoRow();
	
	TArray<FInventoryItemInfoRow*> ItemInfoRows;
	
	ItemDataTable->GetAllRows<FInventoryItemInfoRow>("", ItemInfoRows);

	if (ItemInfoRows.Num() > 0)
	{
		for (FInventoryItemInfoRow* ItemInfoRow : ItemInfoRows)
		{
			if (ItemInfoRow && ItemInfoRow->ItemTag.MatchesTagExact(ItemTag))
				return *ItemInfoRow;
		}
	}
	
	return FInventoryItemInfoRow();
}

FText ARGameMode::GetItemNameWithTag(const FGameplayTag ItemTag) const
{
	FInventoryItemInfoRow InfoRow = GetItemInfoWithTag(ItemTag);

	return InfoRow.ItemName;
}

FText ARGameMode::GetItemDescriptionWithTag(const FGameplayTag ItemTag) const
{
	FInventoryItemInfoRow InfoRow = GetItemInfoWithTag(ItemTag);

	return InfoRow.ItemDescription;
}

void ARGameMode::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

float ARGameMode::GetSavedMasterVolume()
{
	return CurrentSaveGame->MasterVolume;
}

float ARGameMode::GetSavedMusicVolume()
{
	return CurrentSaveGame->MusicVolume;
}

float ARGameMode::GetSavedSFXVolume()
{
	return CurrentSaveGame->SFXVolume;
}

float ARGameMode::GetSavedVoiceVolume()
{
	return CurrentSaveGame->VoiceVolume;
}

void ARGameMode::PlaySequenceByName(FName SequenceName, bool bRestoreState)
{
	ULevelSequence* CurrentSequence = Sequences.Find(SequenceName)->LoadSynchronous();

	if (!CurrentSequence) return;

	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = true;
	Settings.bRestoreState = bRestoreState;

	SequenceActor = nullptr;
	ULevelSequencePlayer::CreateLevelSequencePlayer(this, CurrentSequence, Settings, SequenceActor);
}

void ARGameMode::LoadSaveGame()
{
	CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (CurrentSaveGame == nullptr)
	{
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));
		CurrentSaveGame->bSubtitles = true;
		CurrentSaveGame->bCameraShakeEnabled = true;
		CurrentSaveGame->bFirstLaunch = true;
		CurrentSaveGame->MasterVolume = 1.0f;
		CurrentSaveGame->MusicVolume = 1.0f;
		CurrentSaveGame->VoiceVolume = 1.0f;
		CurrentSaveGame->SFXVolume = 1.0f;
		CurrentSaveGame->CameraSensitivity = 1.0f;
		
		UE_LOG(LogTemp, Log, TEXT("Create new SaveGame Data."));
	}
}

void ARGameMode::ExecuteSpecialEventOnInventoryItemAdded(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex)
{
	for (FSpecialEventData& EventData : SpecialEvents)
	{
		if (EventData.EventInstigationSetting == OnItemPickUp && EventData.StartTags.HasTagExact(ItemTag))
		{
			PlaySequenceByName(EventData.SequenceNameToPlay, true);
		}
	}
}

void ARGameMode::ExecuteSpecialEventOnInventoryItemUsed(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex)
{
	for (FSpecialEventData& EventData : SpecialEvents)
	{
		if (EventData.EventInstigationSetting == OnItemUsed && EventData.StartTags.HasTagExact(ItemTag))
		{
			PlaySequenceByName(EventData.SequenceNameToPlay, true);
		}
	}
}
