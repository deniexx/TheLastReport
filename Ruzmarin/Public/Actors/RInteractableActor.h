#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/RGameplayInterface.h"
#include "RInteractableActor.generated.h"

UCLASS()
class RUZMARIN_API ARInteractableActor : public AActor , public IRGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARInteractableActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* InteractSound;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	virtual USceneComponent* GetAttachComponent_Implementation() override;
};
