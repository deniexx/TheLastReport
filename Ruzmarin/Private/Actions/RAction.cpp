


#include "Actions/RAction.h"

#include "Components/RActionComponent.h"

void URAction::Initialize(URActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

bool URAction::IsRunning() const
{
	return bIsRunning;
}

bool URAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;

	const URActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAnyExact(BlockingTags))
		return false;

	return true;
}

void URAction::StartAction_Implementation(AActor* Instigator)
{
	URActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
	InstigatorActor = Instigator;
}

void URAction::StopAction_Implementation(AActor* Instigator)
{
	URActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
	InstigatorActor = nullptr;
}

UWorld* URAction::GetWorld() const
{
	const AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

URActionComponent* URAction::GetOwningComponent() const
{
	return ActionComponent;
}
