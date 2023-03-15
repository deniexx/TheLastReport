

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RSaveGame.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "RGameMode.generated.h"

UENUM(BlueprintType)
enum ESpecialEventInstigationSetting
{
	None,
	OnItemPickUp,
	OnItemUsed
};

USTRUCT(BlueprintType)
struct FInventoryItemInfoRow : public FTableRowBase
{
	GENERATED_BODY()
	
	FInventoryItemInfoRow()
	{
		ItemName = FText::FromString("Item Name Goes Here!");
		ItemDescription = FText::FromString("Item Description Goes Here!");
		MaxStackAmount = 1;
		bCanBeUsed = true;
		bCanBeExamined = true;
		bCanBeDropped = true;
		bHasAction = false;
	}

	/* ItemTag to be used to search for the item, should be the same as the one in Action_InventoryPickUp */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ItemTag;

	/* Primary Asset Id, holding information about inventory items */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId ItemID;
	
	/* The name of the item to be displayed in inventory menu */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;
	
	/* The description of the item to be displayed in inventory menu */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;

	/* Maximum amount of items in a stack, if 1, item is not stackable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 MaxStackAmount;

	/* Can the item be used? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanBeUsed;

	/* Can the item be examined? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanBeExamined;

	/* Can the item be dropped? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanBeDropped;

	/* Does the item have action assigned in the Primary Data Asset? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasAction;
	
};

USTRUCT(BlueprintType)
struct FSpecialEventData
{
	GENERATED_BODY()
	
	/* How does the event begin */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ESpecialEventInstigationSetting> EventInstigationSetting;

	/* Which tags can begin this special event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer StartTags;

	/* Which sequence to play when special event has been triggered */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SequenceNameToPlay;
};

/**
 * 
 */
UCLASS()
class RUZMARIN_API ARGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void SetUpInventoryBindings(class URInventoryComponent* InventoryComponent);

	void RandomizeItems();

	UFUNCTION(BlueprintCallable, Category= "GameMode" )
	void PlayGame();
	
	UFUNCTION()
	FInventoryItemInfoRow GetItemInfoWithTag(const FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Items")
	FText GetItemNameWithTag(const FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Items")
	FText GetItemDescriptionWithTag(const FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	float GetSavedMasterVolume();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	float GetSavedMusicVolume();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	float GetSavedSFXVolume();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	float GetSavedVoiceVolume();

	UPROPERTY(BlueprintReadOnly)
	URSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable, Category = "Sequences")
	void PlaySequenceByName(FName SequenceName, bool bRestoreState);

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void CloseLoadingScreenWidget();

protected:

	UFUNCTION()
	void ExecuteSpecialEventOnInventoryItemAdded(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex);

	UFUNCTION()
	void ExecuteSpecialEventOnInventoryItemUsed(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex);

	void LoadSaveGame();

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString SlotName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Sequencer")
	TMap<FName, TSoftObjectPtr<class ULevelSequence>> Sequences;

	UPROPERTY(EditDefaultsOnly, Category = "Special Events")
	TArray<FSpecialEventData> SpecialEvents;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<APlayerController> CustomPlayerControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<APawn> CustomDefaultPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Loading")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	class ULoadingScreenWidget* LoadingWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Player")
	FTransform PlayerSpawnTransform;

	UPROPERTY()
	class ALevelSequenceActor* SequenceActor;

	UFUNCTION()
	void GetReadyForPlay();
	
	void SpawnActorAtBestLocation(class ARInventoryPickUpActor* ActorToSpawn, TArray<class ARRandomLocationActor*>* RandomLocationActors);
};
