#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "RGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RUZMARIN_API IRGameplayInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActorFocused();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActorLostFocus();

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AfterInventoryItemUsed(APawn* InstigatorPawn, FGameplayTag ItemUsed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USceneComponent* GetAttachComponent();
};
