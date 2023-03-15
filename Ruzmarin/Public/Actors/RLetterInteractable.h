

#pragma once

#include "CoreMinimal.h"
#include "Actors/RInteractableActor.h"
#include "RLetterInteractable.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API ARLetterInteractable : public ARInteractableActor
{
	GENERATED_BODY()

public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Letter", meta = ( MultiLine="true" ))
	TArray<FText> TextOnPages;
};
