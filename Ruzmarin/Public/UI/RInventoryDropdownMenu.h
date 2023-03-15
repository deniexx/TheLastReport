

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RInventoryDropdownMenu.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URInventoryDropdownMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class USizeBox* ParentSizeBox;
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UButton* UseItemButton;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	UButton* DropItemButton;

	UPROPERTY(Transient, EditAnywhere, meta = ( BindWidgetAnim ))
	UWidgetAnimation* OpenDropdown;
};
