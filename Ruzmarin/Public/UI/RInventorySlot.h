

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RInventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotClicked, uint8, SlotIndex);

/**
 * 
 */
UCLASS()
class RUZMARIN_API URInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Slot")
	uint8 SlotIndex;
	
	void UpdateSlot(UTexture2D* ItemIcon, uint8 NewAmount) const;

	UPROPERTY()
	FOnInventorySlotClicked OnInventorySlotClicked;

protected:

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class USizeBox* ParentSizeBox;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UOverlay* SlotOverlay;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UImage* SlotIcon;
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UButton* SlotButton;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UTextBlock* ItemAmount;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickEvent();
};
