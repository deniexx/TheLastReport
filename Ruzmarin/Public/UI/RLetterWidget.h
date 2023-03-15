

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RLetterWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URLetterWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Sentences")
	void SetSentences(TArray<FText> NewSentences);

	void ResetWidget();

	virtual void NativeConstruct() override;
	
protected:
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UButton* NextSentenceButton;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	UButton* PreviousSentenceButton;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	UButton* ReadText;
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	UButton* CloseWidget;
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UTextBlock* MainText;

	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	UTextBlock* ReadableText;
	
	UPROPERTY(EditAnywhere, meta = ( BindWidget ))
	class UImage* Background;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* ShowText;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* ShowReadable;
	
	FOnInputAction CloseWidgetAction;
	
	TArray<FText> Sentences;

	int ArrayIndex;

	UPROPERTY()
	bool bIsReadableOpen;

	UFUNCTION()
	void ShowNextSentence();

	UFUNCTION()
	void ShowPreviousSentence();

	UFUNCTION()
	void ShowReadableText();

	UFUNCTION()
	void ExitWidget();
};
