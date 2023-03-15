


#include "Actions/Action_Flashlight.h"

#include "Actors/RCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/RActionComponent.h"
#include "Components/RAttributeComponent.h"
#include "GameFramework/Character.h"

UAction_Flashlight::UAction_Flashlight()
{
	bIsFlashlightOn = false;

	FlashlightOffset = FVector(-20, 50, -10);
}

void UAction_Flashlight::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	if (FlashlightInstance == nullptr)
	{
		FlashlightInstance = GetWorld()->SpawnActor(Flashlight);

		if (FlashlightInstance)
		{
			ARCharacter* MyChar = Cast<ARCharacter>(Instigator);
			if (MyChar)
			{
				FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::SnapToTarget, false};
				FlashlightInstance->AttachToComponent(MyChar->GetCameraComponent(), AttachmentTransformRules);
				bIsFlashlightOn = true;

				FVector TempLocation = MyChar->GetCameraComponent()->GetRelativeLocation();
				FlashlightInstance->SetActorRelativeLocation(TempLocation + FlashlightOffset);
				
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ReduceBatteryPercentage_Timer", Instigator);
				GetWorld()->GetTimerManager().SetTimer(ReduceBatteryPercentage_Handle, Delegate, BatteryDrainTick, true);
				
				return;
			}
			UE_LOG(LogTemp, Warning, TEXT("Couldn't find actor to attach to: Action_Flashlight"))
		}
	}

	if (bIsFlashlightOn)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReduceBatteryPercentage_Handle);
		FlashlightInstance->SetActorHiddenInGame(true);
		bIsFlashlightOn = false;
	}
	else
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ReduceBatteryPercentage_Timer", Instigator);
		GetWorld()->GetTimerManager().SetTimer(ReduceBatteryPercentage_Handle, Delegate, BatteryDrainTick, true);
		
		FlashlightInstance->SetActorHiddenInGame(false);
		bIsFlashlightOn = true;
	}
	
}

void UAction_Flashlight::ReduceBatteryPercentage_Timer(AActor* Instigator)
{
	URAttributeComponent* Attributes = URAttributeComponent::GetAttributes(Instigator);

	if (Attributes)
	{
		Attributes->ApplyAttributeChange(Instigator, BatteryAttributeTag, -BatteryDrainAmount);

		if (Attributes->GetAttributeValue(BatteryAttributeTag) <= 0.0f && bIsFlashlightOn)
		{
			StartAction(Instigator);
		}
	}
}

void UAction_Flashlight::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	URActionComponent* MyActionComponent = URActionComponent::GetActionComponent(Instigator);
	if (MyActionComponent)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReduceBatteryPercentage_Handle);
		MyActionComponent->RemoveAction(ActionTag);
		FlashlightInstance->Destroy();
	}
}

bool UAction_Flashlight::CanStart_Implementation(AActor* Instigator)
{
	URAttributeComponent* Attributes = URAttributeComponent::GetAttributes(Instigator);
	if (Attributes && Attributes->GetAttributeValue(BatteryAttributeTag) > 0.0f || bIsFlashlightOn)
	{
		return true;
	}

	return false;
}
