

#pragma once

#include "CoreMinimal.h"
#include "Actions/RAction.h"
#include "Action_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API UAction_Sprint : public URAction
{
	GENERATED_BODY()

public:
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	virtual bool CanStart_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	FGameplayTag SprintTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float SpeedIncrease;

	/** How much stamina to drain per tick, should be a positive number */
	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float StaminaDrain;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float StaminaDrainPeriod;

	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float StaminaRecoveryPeriod;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action Stats")
	float StaminaRecoveryDelay;

	UPROPERTY()
	FTimerHandle ReduceStamina_Handle;

	UPROPERTY()
	FTimerHandle RecoverStamina_Handle;

	UFUNCTION()
	void ReduceStamina_Timer(AActor* Instigator);

	UFUNCTION()
	void RecoverStamina_Timer(AActor* Instigator);
};
