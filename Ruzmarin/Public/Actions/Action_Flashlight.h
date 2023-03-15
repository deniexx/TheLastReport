

#pragma once

#include "CoreMinimal.h"
#include "Actions/RAction.h"
#include "Action_Flashlight.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API UAction_Flashlight : public URAction
{
	GENERATED_BODY()
public:

	UAction_Flashlight();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	TSubclassOf<AActor> Flashlight;

	UPROPERTY()
	AActor* FlashlightInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	FVector FlashlightOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	FGameplayTag BatteryAttributeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float BatteryDrainTick;

	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float BatteryDrainAmount;

	bool bIsFlashlightOn;

	UPROPERTY()
	FTimerHandle ReduceBatteryPercentage_Handle;

	UFUNCTION()
	void ReduceBatteryPercentage_Timer(AActor* Instigator);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	virtual bool CanStart_Implementation(AActor* Instigator) override;
};
