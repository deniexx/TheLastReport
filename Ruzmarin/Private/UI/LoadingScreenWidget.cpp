


#include "UI/LoadingScreenWidget.h"

#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"

void ULoadingScreenWidget::ShowTextBlock()
{
	bCanClose = true;
	CircularThrobber->SetVisibility(ESlateVisibility::Collapsed);
	TextBlock->SetVisibility(ESlateVisibility::Visible);
}
