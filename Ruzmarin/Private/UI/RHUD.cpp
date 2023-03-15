


#include "UI/RHUD.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RAttributeComponent.h"

void URHUD::UpdateVisibility(EHUDVisibilityOption NewVisibility) const
{
	switch (NewVisibility) {
		case OnlyAttributeBar:
			CrosshairImage->SetVisibility(ESlateVisibility::Collapsed);
			AttributesBox->SetVisibility(ESlateVisibility::Visible);
			break;
		case OnlyCrosshair:
			CrosshairImage->SetVisibility(ESlateVisibility::Visible);
			AttributesBox->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case AllVisible:
			CrosshairImage->SetVisibility(ESlateVisibility::Visible);
			AttributesBox->SetVisibility(ESlateVisibility::Visible);
			break;
		case AllCollapsed:
			CrosshairImage->SetVisibility(ESlateVisibility::Collapsed);
			AttributesBox->SetVisibility(ESlateVisibility::Collapsed);
			break;
		default: break;
	}
}

void URHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	APawn* MyPawn = GetOwningPlayerPawn();
	if (MyPawn)
	{
		URAttributeComponent* Attributes = URAttributeComponent::GetAttributes(MyPawn);
		if (Attributes)
		{
			Attributes->OnAttributeChanged.AddDynamic(this, &URHUD::OnAttributeChanged);

			const FAttribute StaminaAttribute = Attributes->GetAttribute(StaminaTag);
			const float NewSValue = StaminaAttribute.CurrentValue / StaminaAttribute.MaxValue;
			StaminaBar->SetPercent(NewSValue);
		}
	}
}

void URHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bUpdateStaminaValue)
	{
		StaminaLerpValue += InDeltaTime * 2;
		const float CurrentAlpha = FMath::Lerp(OldStaminaValue, NewStaminaValue, StaminaLerpValue);

		StaminaBar->SetPercent(CurrentAlpha);
		if (StaminaLerpValue >= 1.0f)
		{
			bUpdateStaminaValue = false;
			
			GetWorld()->GetTimerManager().SetTimer(Handle_HideStaminaBar, this, &URHUD::HideStaminaBar ,StaminaBarStayDuration);
		}
	}
}

void URHUD::OnAttributeChanged(AActor* Instigator, URAttributeComponent* OwningComponent, FGameplayTag AttributeTag, float NewValue, float Delta)
{
	const float MaxValue = OwningComponent->GetAttributeMax(AttributeTag);
	
	if (AttributeTag.MatchesTagExact(StaminaTag))
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle_HideStaminaBar);
		StaminaBar->SetVisibility(ESlateVisibility::Visible);
		
		if (bUpdateStaminaValue)
		{
			OldStaminaValue = FMath::Lerp(OldStaminaValue, NewStaminaValue, StaminaLerpValue);
			NewStaminaValue = NewValue / MaxValue;
			StaminaLerpValue = 0;
			return;
		}
		
		NewStaminaValue = NewValue / MaxValue;
		OldStaminaValue = (NewValue - Delta) / MaxValue;
		StaminaLerpValue = 0;
		bUpdateStaminaValue = true;
	}
}

void URHUD::HideStaminaBar()
{
	StaminaBar->SetVisibility(ESlateVisibility::Collapsed);
}
