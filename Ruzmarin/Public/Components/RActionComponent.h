

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUZMARIN_API URActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URActionComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actions")
	static URActionComponent* GetActionComponent(AActor* FromActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<class URAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByTag(AActor* Instigator, FGameplayTag StartActionTag);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByTag(AActor* Instigator, FGameplayTag StopActionTag);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(FGameplayTag ActionToRemove);

	UFUNCTION()
	URAction* GetAction(TSubclassOf<URAction> ActionClass);

protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<URAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly, Category = "Actions")
	TArray<URAction*> Actions;
};
