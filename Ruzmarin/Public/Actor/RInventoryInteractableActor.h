

#pragma once

#include "CoreMinimal.h"
#include "Actors/RInteractableActor.h"
#include "RInventoryInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API ARInventoryInteractableActor : public ARInteractableActor
{
	GENERATED_BODY()

public:

	ARInventoryInteractableActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
	FGameplayTagContainer AcceptedGameplayTags;

	UPROPERTY(EditAnywhere, Category = "Highlight Item")
	UStaticMeshComponent* HighlightMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory Item")
	bool bInteractionDone;

	virtual void OnActorFocused_Implementation() override;

	virtual void OnActorLostFocus_Implementation() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
