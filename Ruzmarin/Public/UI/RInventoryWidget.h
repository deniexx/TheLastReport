

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RInventoryDropdownMenu.h"
#include "Blueprint/UserWidget.h"
#include "RInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void HideDropDownMenu();

	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory UI")
	TSubclassOf<class UUserWidget> InventorySlotClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Ui")
	UTexture2D* EmptySlotTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Ui")
	float XDropDownOffset = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Ui")
	float YDropDownOffset = 30;
 
	UPROPERTY(BlueprintReadOnly, Category = "Inventory UI")
	TArray<class URInventorySlot*> InventorySlots;

	/* ---------------------- BIND WIDGETS ---------------------- */
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class USizeBox* ParentInventorySizeBox;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UUniformGridPanel* InventoryUniformGrid;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	UTextBlock* ItemDescription;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UImage* CloseDropdownImage;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class URInventoryDropdownMenu* DropDownMenu;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* OpenItemText;

	/* ---------------------- END BIND WIDGETS ---------------------- */

	UPROPERTY()
	uint8 CurrentSlotSelected;

	UFUNCTION()
	FEventReply CloseDropdownMenu(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnInventoryItemAdded(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex);
	
	UFUNCTION()
	void OnInventoryItemUsed(FGameplayTag ItemTag, UTexture2D* ItemTexture, uint8 ItemAmount, uint8 SlotIndex);

	UFUNCTION()
	void OnUseItemClicked();

	UFUNCTION()
	void OnDropItemClicked();

	UFUNCTION()
	void OnSlotClicked(uint8 SlotIndex);
};
