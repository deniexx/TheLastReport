

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePaused, bool, bIsPaused);

UENUM(BlueprintType)
enum EHUDVisibilityOption
{
	OnlyAttributeBar,
	OnlyCrosshair,
	AllVisible,
	AllCollapsed,
};

/**
 * 
 */
UCLASS()
class RUZMARIN_API ARPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleHUD(EHUDVisibilityOption NewVisibility) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetIsInAnyUI(bool NewValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	bool GetIsPlayerInAnyUI();

	UFUNCTION(BlueprintCallable, Category = "Letter")
	void OpenLetterWidget(TArray<FText> Sentences);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void ClosePauseMenu();

	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Pause")
	FOnGamePaused OnGamePaused;

	void AfterLoad();

	void StopAll(bool bNewState);
	
protected:
	
	UFUNCTION()
	void TryPauseGame();

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> DefaultHUDClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> LetterWidgetClass;

	UPROPERTY()
	class URLetterWidget* LetterWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseMenuWidget;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UPROPERTY()
	class URHUD* HUDInstance;

private:

	UPROPERTY()
	bool bIsInAnyUI;
};
