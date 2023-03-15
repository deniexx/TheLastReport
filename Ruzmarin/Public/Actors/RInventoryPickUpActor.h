

#pragma once

#include "CoreMinimal.h"
#include "Actors/RInteractableActor.h"
#include "RInventoryPickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API ARInventoryPickUpActor : public ARInteractableActor
{
	GENERATED_BODY()

	ARInventoryPickUpActor();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Item")
	FGameplayTag ItemTag;

	void SetMesh(UStaticMesh* NewMesh) const;

	void SetScaleAndSphereRadius(FVector NewScale, FVector BoxExtends, FVector BoxLocation) const;

	void SetPhysicsOn();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	/* 0 - Smallest Items : 5 - Very Large */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 ItemSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer AllowedSpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRandomizeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnablePhysicsOnStart;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle TurnOffPhysics_Handle;

	UFUNCTION()
	void TurnOffPhysics();
	
};
