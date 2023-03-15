#include "Actors/RInteractableActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARInteractableActor::ARInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

void ARInteractableActor::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InteractSound)
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
}

FText ARInteractableActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

USceneComponent* ARInteractableActor::GetAttachComponent_Implementation()
{
	return BoxComponent;
}


