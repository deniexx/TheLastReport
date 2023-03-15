

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/RPlayerController.h"
#include "RHUD.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( BindWidget ))
	class UProgressBar* StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( BindWidget ))
	class UImage* CrosshairImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( BindWidget ))
	class UHorizontalBox* AttributesBox;

	void UpdateVisibility(EHUDVisibilityOption NewVisibility) const;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnAttributeChanged(AActor* Instigator, class URAttributeComponent* OwningComponent, FGameplayTag AttributeTag, float NewValue, float Delta);
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag StaminaTag;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float StaminaBarStayDuration = 2.f;

	UPROPERTY()
	FTimerHandle Handle_HideStaminaBar;

private:

	UFUNCTION()
	void HideStaminaBar();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FName AlphaParameterName = "Alpha";
	
	/* --------------- STAMINA VARIABLES --------------- */
	bool bUpdateStaminaValue;
	float NewStaminaValue;
	float OldStaminaValue;
	float StaminaLerpValue;
};
