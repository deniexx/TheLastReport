

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RUZMARIN_API URAction : public UObject
{
	GENERATED_BODY()


public:

	void Initialize(class URActionComponent* NewActionComponent);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTag ActionTag;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	AActor* GetInstigator() { return InstigatorActor; }

protected:

	UPROPERTY()
	URActionComponent* ActionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockingTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	URActionComponent* GetOwningComponent() const;

	UPROPERTY(BlueprintReadOnly, Category = "Action")
	AActor* InstigatorActor;

	UPROPERTY()
	bool bIsRunning;
	
};
