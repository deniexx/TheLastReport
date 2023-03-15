


#include "Actors/RInventoryPickUpActor.h"

#include "Components/BoxComponent.h"
#include "Components/RInventoryComponent.h"
#include "Components/SphereComponent.h"


ARInventoryPickUpActor::ARInventoryPickUpActor()
{
	ItemSize = 0;
	bRandomizeLocation = false;
	bEnablePhysicsOnStart = false;
}

void ARInventoryPickUpActor::BeginPlay()
{
	Super::BeginPlay();

	if (bEnablePhysicsOnStart)
		SetPhysicsOn();
}

void ARInventoryPickUpActor::SetMesh(UStaticMesh* NewMesh) const
{
	BaseMesh->SetStaticMesh(NewMesh);
}

void ARInventoryPickUpActor::SetScaleAndSphereRadius(FVector NewScale, FVector BoxExtends, FVector BoxLocation) const
{
	BaseMesh->SetWorldScale3D(NewScale);
	BoxComponent->SetBoxExtent(BoxExtends);
	BoxComponent->SetRelativeLocation(BoxLocation);
}

void ARInventoryPickUpActor::SetPhysicsOn()
{
	BaseMesh->SetSimulatePhysics(true);
	
	GetWorld()->GetTimerManager().SetTimer(TurnOffPhysics_Handle, this, &ARInventoryPickUpActor::TurnOffPhysics, 2.0f);
}

void ARInventoryPickUpActor::TurnOffPhysics()
{
	BaseMesh->SetSimulatePhysics(false);
}

void ARInventoryPickUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	
	URInventoryComponent* Inventory = URInventoryComponent::GetInventoryComponent(InstigatorPawn);

	if (Inventory)
	{
		if (Inventory->AddItemToInventory(ItemTag))
		{
			Destroy();
		}
	}
}
