

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAttributeChanged, AActor*, Instigator, URAttributeComponent*, OwningComponent, FGameplayTag, AttributeTag, float, NewValue, float, Delta);

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	FGameplayTagContainer ChangeBlockingAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxValue = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float CurrentValue = 100;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUZMARIN_API URAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URAttributeComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	static URAttributeComponent* GetAttributes(AActor* FromActor);

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnAttributeChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyAttributeChange(AActor* InstigatorActor, FGameplayTag AttributeToChange, float Delta, bool bOverrideValue = false);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	FAttribute GetAttribute(FGameplayTag AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAttributeMax(FGameplayTag AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeValue(FGameplayTag AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttributeMax(FGameplayTag AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<FAttribute> Attributes;
};
