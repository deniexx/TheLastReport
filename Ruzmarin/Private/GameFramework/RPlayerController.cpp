


#include "GameFramework/RPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/RInventoryComponent.h"
#include "UI/RHUD.h"
#include "UI/RLetterWidget.h"


void ARPlayerController::ToggleHUD(EHUDVisibilityOption NewVisibility) const
{
	HUDInstance->UpdateVisibility(NewVisibility);
}

void ARPlayerController::SetIsInAnyUI(const bool NewValue)
{
	bIsInAnyUI = NewValue;
}

bool ARPlayerController::GetIsPlayerInAnyUI()
{
	return bIsInAnyUI;
}

void ARPlayerController::OpenLetterWidget(TArray<FText> Sentences)
{
	if (bIsInAnyUI) return;
	
	if (!LetterWidgetInstance)
	{
		LetterWidgetInstance = CreateWidget<URLetterWidget>(this, LetterWidgetClass);
		LetterWidgetInstance->SetSentences(Sentences);
		LetterWidgetInstance->AddToViewport();
	}
	else
	{
		LetterWidgetInstance->SetSentences(Sentences);
		if (!LetterWidgetInstance->IsInViewport())
			LetterWidgetInstance->AddToViewport();
	}

	SetInputMode(FInputModeGameAndUI());
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	bShowMouseCursor = true;

	bIsInAnyUI = true;
}

void ARPlayerController::ClosePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		SetInputMode(FInputModeGameOnly());
		SetPause(false);
		bShowMouseCursor = false;
		bIsInAnyUI = false;
		OnGamePaused.Broadcast(false);
	}
}

void ARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputActionBinding& PauseAction = InputComponent->BindAction("Pause", IE_Pressed, this, &ARPlayerController::TryPauseGame);
	PauseAction.bExecuteWhenPaused = true;
}

void ARPlayerController::AfterLoad()
{
	if (HUDInstance == nullptr)
	{
		HUDInstance = CreateWidget<URHUD>(this, DefaultHUDClass);
		HUDInstance->AddToViewport();
	}
}

void ARPlayerController::StopAll(bool bNewState)
{
	if (bNewState)
		SetInputMode(FInputModeUIOnly());
	else
		SetInputMode(FInputModeGameOnly());

	
	SetIgnoreMoveInput(bNewState);
	SetIgnoreLookInput(bNewState);
	bShowMouseCursor = bNewState;
}

void ARPlayerController::TryPauseGame()
{
	if (bIsInAnyUI)
	{
		if (LetterWidgetInstance && LetterWidgetInstance->IsInViewport())
		{
			LetterWidgetInstance->RemoveFromParent();
			SetIsInAnyUI(false);
			SetInputMode(FInputModeGameOnly());
			SetIgnoreMoveInput(false);
			SetIgnoreLookInput(false);
			bShowMouseCursor = false;
			return;
		}

		if(URInventoryComponent* InventoryComponent = URInventoryComponent::GetInventoryComponent(GetCharacter()))
		{
			if (InventoryComponent->IsInventoryVisible())
			{
				InventoryComponent->ToggleInventory(GetOwner());
				return;
			}
		}
	}

	if(ensure(PauseMenuWidget))
	{
		if (!PauseMenuInstance)
		{
			PauseMenuInstance = CreateWidget(this, PauseMenuWidget);
		}

		if (PauseMenuInstance)
		{
			if (!PauseMenuInstance->IsInViewport())
			{
				PauseMenuInstance->AddToViewport(100);
				SetInputMode(FInputModeGameAndUI());
				SetPause(true);
				bShowMouseCursor = true;
				OnGamePaused.Broadcast(true);
			}
			else
			{
				ClosePauseMenu();
			}
		}
	}

	bIsInAnyUI = false;
}
