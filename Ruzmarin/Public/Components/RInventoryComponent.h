

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Actors/RCharacter.h"
#include "Components/ActorComponent.h"
#include "GameFramework/RGameMode.h"
#include "RInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInventoryItemChange, FGameplayTag, ItemTag, UTexture2D*, ItemTexture, uint8, ItemAmount, uint8, SlotIndex);

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item")
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item")
	uint8 Amount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item")
	uint8 SlotIndex = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUZMARIN_API URInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URInventoryComponent();

	void SetUp();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static URInventoryComponent* GetInventoryComponent(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FGameplayTag GetItemTagAtSlot(uint8 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(FGameplayTag ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseInventoryItem(uint8 ItemSlot);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropInventoryItem(uint8 ItemSlot);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryItemChange OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryItemChange OnInventoryItemUsed;

	/* Max slots to ever be available in the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	uint8 InventoryMaxSlots;

	/* How many inventory slots should be displayed per Row */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	uint8 InventorySlotsPerRow;

	/* All the slots available to used in the inventory, does not account for occupied slots */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	uint8 AvailableInventorySlots;

	/* Toggle inventory visibility */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag EmptyItemSlotTag;

	bool IsInventoryVisible() const;

protected:

	UPROPERTY()
	ARCharacter* CharacterOwner;

	/* Inventory Widget Class to be constructed and used */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory UI")
	TSubclassOf<class UUserWidget> InventoryWidgetClass;

	/* The current instance of our InventoryWidget class */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory UI")
	class URInventoryWidget* InventoryWidgetInstance;

	/* The drop range, to check ahead for free space to drop item */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory UI")
	float DropRange;

	/* How high should the item be offset from the center of the capsule */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory UI")
	float ActorEyesOffset;

	/* Base class for the InventoryPickUp class, used for dropping items */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Items")
	TSubclassOf<class AActor> InventoryPickUpActorClass;
	
	/* Items in the inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryItem> InventoryItems;
	
	/* Occupied slots in the inventory */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	uint8 OccupiedSlots;
	
	UPROPERTY()
	AActor* InstigatorActor;

	UFUNCTION()
	void OnAddInventoryItemLoaded(FPrimaryAssetId LoadedID, FInventoryItemInfoRow InfoRow);

	UFUNCTION()
	void OnUseItemLoaded(FPrimaryAssetId LoadedID, FInventoryItemInfoRow InfoRow, uint8 ItemSlot);

	UFUNCTION()
	void OnDropItemLoaded(FPrimaryAssetId LoadedID, FInventoryItemInfoRow InfoRow, uint8 ItemSlot);
	
private:
	
    uint8 FindBestSlotForItemWithTag(const FGameplayTag& ItemTag, const uint8 MaxStackAmount);

	bool CheckCanItemBeAddedToInventory(const FGameplayTag& ItemTag, const uint8 MaxStackAmount);
};
