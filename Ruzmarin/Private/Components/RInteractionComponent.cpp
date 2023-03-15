#include "Components/RInteractionComponent.h"

#include "Interfaces/RGameplayInterface.h"
#include "Ruzmarin/Ruzmarin.h"
#include "UI/RWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("rz.DebugDrawInteraction"), false, TEXT("Draw debug sphere for line interaction."), ECVF_Cheat);

// Sets default values for this component's properties
URInteractionComponent::URInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceSphereRadius = 30.0f;
	TraceDistance = 300.0f;
	TraceCollisionChannel = ECC_WorldDynamic;
}

void URInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		LogOnScreen(GetOwner(), TEXT("No Focused Actor to interact with"));
		UE_LOG(LogTemp, Log, TEXT("No Focused Actor to interact with"))
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	IRGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
	FocusedActor = nullptr;
}

void URInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceCollisionChannel);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	FHitResult Hit;
	FCollisionShape Shape;
	Shape.SetSphere(TraceSphereRadius);

	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	bool bFoundInteractable = false;
	
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceSphereRadius, 16, LineColor, false, 2.0f, 0, 2.0f);
		}

		if (HitActor->Implements<URGameplayInterface>())
		{
			if (HitActor != FocusedActor)
			{
				if (DefaultWidgetInstance)
				{
					DefaultWidgetInstance->RemoveFromParent();
					DefaultWidgetInstance = nullptr;
				}

				if (FocusedActor)
					IRGameplayInterface::Execute_OnActorLostFocus(FocusedActor);
				
				IRGameplayInterface::Execute_OnActorFocused(HitActor);
			}
			bFoundInteractable = true;
			FocusedActor = HitActor;
		}
	}

	if (!bBlockingHit || !bFoundInteractable)
	{
		if (FocusedActor)
			IRGameplayInterface::Execute_OnActorLostFocus(FocusedActor);

		FocusedActor = nullptr;
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
			DefaultWidgetInstance = CreateWidget<URWorldUserWidget>(GetWorld(), DefaultWidgetClass);

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			DefaultWidgetInstance->AttachedComponent = IRGameplayInterface::Execute_GetAttachComponent(FocusedActor);

			if (!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport(-1);
		}
	}
	else
	{
		if (DefaultWidgetInstance)
			DefaultWidgetInstance->RemoveFromParent();
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0.0f, 2.0f);
	}
}

void URInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

