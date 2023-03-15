


#include "Actor/RInventoryInteractableActor.h"

#include "Components/RInventoryComponent.h"

ARInventoryInteractableActor::ARInventoryInteractableActor()
{
	HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Highlight Mesh"));
	HighlightMesh->SetupAttachment(BaseMesh);
	bInteractionDone = false;
}

void ARInventoryInteractableActor::OnActorFocused_Implementation()
{
	HighlightMesh->SetVisibility(true);
}

void ARInventoryInteractableActor::OnActorLostFocus_Implementation()
{
	HighlightMesh->SetVisibility(false);
}

void ARInventoryInteractableActor::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (bInteractionDone) return;
	
	URInventoryComponent* InventoryComponent = URInventoryComponent::GetInventoryComponent(InstigatorPawn);

	if (InventoryComponent)
		InventoryComponent->ToggleInventory(this);
}

