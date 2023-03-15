


#include "UI/RLetterWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/RPlayerController.h"


void URLetterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NextSentenceButton->OnClicked.AddDynamic(this, &URLetterWidget::ShowNextSentence);
	PreviousSentenceButton->OnClicked.AddDynamic(this, &URLetterWidget::ShowPreviousSentence);
	ReadText->OnClicked.AddDynamic(this, &URLetterWidget::ShowReadableText);
	CloseWidget->OnClicked.AddDynamic(this, &URLetterWidget::ExitWidget);
}

void URLetterWidget::SetSentences(TArray<FText> NewSentences)
{
	ResetWidget();
	
	for (FText& Sentence : NewSentences)
	{
		Sentences.Add(Sentence);
	}

	CloseWidgetAction.BindDynamic(this, &URLetterWidget::ExitWidget);
	ListenForInputAction("Exit", IE_Pressed, true, CloseWidgetAction);

	PreviousSentenceButton->SetIsEnabled(false);

	// Show first sentence without playing animation
	MainText->SetText(Sentences[ArrayIndex]);
	ReadableText->SetText(Sentences[ArrayIndex]);

	if (ArrayIndex + 1 == Sentences.Num()) NextSentenceButton->SetIsEnabled(false);
}

void URLetterWidget::ResetWidget()
{
	NextSentenceButton->SetIsEnabled(true);
	Sentences.Empty();
	ArrayIndex = 0;
}

void URLetterWidget::ShowNextSentence()
{
	if (Sentences.Num() == 0) ExitWidget();

	if (!PreviousSentenceButton->GetIsEnabled())
		PreviousSentenceButton->SetIsEnabled(true);

	ArrayIndex += 1;
	
	PlayAnimationForward(ShowText);
	MainText->SetText(Sentences[ArrayIndex]);
	ReadableText->SetText(Sentences[ArrayIndex]);

	if (ArrayIndex + 1 == Sentences.Num()) NextSentenceButton->SetIsEnabled(false);
}

void URLetterWidget::ShowPreviousSentence()
{
	if (Sentences.Num() == 0) ExitWidget();

	if (!NextSentenceButton->GetIsEnabled())
		NextSentenceButton->SetIsEnabled(true);
	
	ArrayIndex -= 1;
	
	PlayAnimationForward(ShowText);
	MainText->SetText(Sentences[ArrayIndex]);
	ReadableText->SetText(Sentences[ArrayIndex]);

	if (ArrayIndex == 0) PreviousSentenceButton->SetIsEnabled(false);
}

void URLetterWidget::ShowReadableText()
{
	if (bIsReadableOpen)
	{
		PlayAnimationReverse(ShowReadable);
	}
	else
	{
		PlayAnimationForward(ShowReadable);
	}

	bIsReadableOpen = !bIsReadableOpen;
}
	

void URLetterWidget::ExitWidget()
{
	ARPlayerController* PC = GetOwningPlayer<ARPlayerController>();

	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->ResetIgnoreInputFlags();
		PC->SetIsInAnyUI(false);
		PC->bShowMouseCursor = false;
		PC->ToggleHUD(EHUDVisibilityOption::AllVisible);
	}
	
	NextSentenceButton->OnClicked.RemoveAll(this);
	PreviousSentenceButton->OnClicked.RemoveAll(this);
	ReadText->OnClicked.RemoveAll(this);
	CloseWidget->OnClicked.RemoveAll(this);
	
	StopListeningForAllInputActions();
	RemoveFromViewport();
}


