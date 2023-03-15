#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUZMARIN_API URInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URInteractionComponent();

	void PrimaryInteract();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceSphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;

	UPROPERTY()
	AActor* FocusedActor;

	UFUNCTION()
	void FindBestInteractable();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class URWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
	URWorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
