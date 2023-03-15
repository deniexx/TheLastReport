


#include "UI/RWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void URWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing World Widget."));
		return;
	}

	FVector2D ScreenPosition;
	bool bIsOnScreen;
	if (AttachedComponent)
		bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedComponent->GetComponentLocation() + WorldOffset, ScreenPosition);
	else
		bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);
	
	if (bIsOnScreen)
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
