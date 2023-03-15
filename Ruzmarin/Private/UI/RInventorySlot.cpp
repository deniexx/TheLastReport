


#include "UI/RInventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"



void URInventorySlot::UpdateSlot(UTexture2D* ItemIcon, uint8 NewAmount) const
{
	SlotIcon->SetBrushFromTexture(ItemIcon);
	ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewAmount)));
}

void URInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotButton->OnReleased.AddDynamic(this, &URInventorySlot::OnClickEvent);
}

void URInventorySlot::OnClickEvent()
{
	OnInventorySlotClicked.Broadcast(SlotIndex);
}
