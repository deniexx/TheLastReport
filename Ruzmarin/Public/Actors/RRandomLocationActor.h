

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "RRandomLocationActor.generated.h"


UCLASS()
class RUZMARIN_API ARRandomLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARRandomLocationActor();
	
	UPROPERTY(EditAnywhere, Category = "Randomizer")
	uint8 ItemSize;

	UPROPERTY(EditAnywhere, Category = "Randomizer")
	FGameplayTag CurrentSpawnLocation;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USphereComponent* SphereComponent;
};
