


#include "Actions/Action_Sprint.h"

#include "Components/RActionComponent.h"
#include "Components/RAttributeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ruzmarin/Ruzmarin.h"

void UAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* MyCharacter = Cast<ACharacter>(Instigator);
	if (MyCharacter)
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed += SpeedIncrease;
		GetWorld()->GetTimerManager().ClearTimer(ReduceStamina_Handle);
		GetWorld()->GetTimerManager().ClearTimer(RecoverStamina_Handle);

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ReduceStamina_Timer", Instigator);
		GetWorld()->GetTimerManager().SetTimer(ReduceStamina_Handle, Delegate, StaminaDrainPeriod, true);
	
		LogOnScreen(GetOwningComponent()->GetOwner(), TEXT("Sprinting"));
	}
}

void UAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	ACharacter* MyCharacter = Cast<ACharacter>(Instigator);
	if (MyCharacter)
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed -= SpeedIncrease;
		GetWorld()->GetTimerManager().ClearTimer(ReduceStamina_Handle);
		GetWorld()->GetTimerManager().ClearTimer(RecoverStamina_Handle);

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RecoverStamina_Timer", Instigator);
		GetWorld()->GetTimerManager().SetTimer(RecoverStamina_Handle, Delegate, StaminaRecoveryPeriod, true, StaminaRecoveryDelay);
	}
}

bool UAction_Sprint::CanStart_Implementation(AActor* Instigator)
{
	bool bCanStart =  Super::CanStart_Implementation(Instigator);

	bool bHasStamina = false;

	URAttributeComponent* Attributes = URAttributeComponent::GetAttributes(Instigator);

	if (Attributes)
	{
		bHasStamina = Attributes->GetAttributeValue(SprintTag) > 0.0f;
	}

	return bHasStamina && bCanStart;
}

void UAction_Sprint::ReduceStamina_Timer(AActor* Instigator)
{
	if (!Instigator) return;

	if (abs(Instigator->GetVelocity().X) <= 1 && abs(Instigator->GetVelocity().Y) <= 1) return;
	
	URAttributeComponent* Attributes = URAttributeComponent::GetAttributes(InstigatorActor);
	
	if (Attributes)
	{
		if (Attributes->GetAttributeValue(SprintTag) > 0.0f)
		{
			Attributes->ApplyAttributeChange(InstigatorActor, SprintTag, -StaminaDrain);

			LogOnScreen(InstigatorActor, FString::Printf(TEXT("Current Stamina: %f"), Attributes->GetAttributeValue(SprintTag)));
		}
		else
		{
			StopAction(Instigator);
		}
	}
}

void UAction_Sprint::RecoverStamina_Timer(AActor* Instigator)
{
	if (!Instigator) return;

	
	
	URAttributeComponent* Attributes = URAttributeComponent::GetAttributes(Instigator);

	if (Attributes)
	{
		if (Attributes->GetAttributeValue(SprintTag) < Attributes->GetAttributeMax(SprintTag))
		{
			Attributes->ApplyAttributeChange(Instigator, SprintTag, StaminaDrain);

			LogOnScreen(Instigator, FString::Printf(TEXT("Current Stamina: %f"), Attributes->GetAttributeValue(SprintTag)));
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ReduceStamina_Handle);
			GetWorld()->GetTimerManager().ClearTimer(RecoverStamina_Handle);
		}
	}
}
