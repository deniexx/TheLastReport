


#include "Components/RActionComponent.h"

#include "Actions/RAction.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "Ruzmarin/Ruzmarin.h"

// Sets default values for this component's properties
URActionComponent::URActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

URActionComponent* URActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URActionComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

// Called when the game starts
void URActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<URAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}

void URActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<URAction*> ActionsCopy = Actions;
	for (URAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void URActionComponent::AddAction(AActor* Instigator, TSubclassOf<URAction> ActionClass)
{
	if (!ensure(ActionClass)) return;

	URAction* NewAction = NewObject<URAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) NewAction->StartAction(Instigator);
	}
}

bool URActionComponent::StartActionByTag(AActor* Instigator, FGameplayTag StartActionTag)
{
	for (URAction* Action : Actions)
	{
		if (Action && Action->ActionTag.MatchesTagExact(StartActionTag))
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run %s"), *StartActionTag.ToString());

				LogOnScreen(GetOwner(), FailedMsg);
				
				continue;
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool URActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag StopActionTag)
{
	for (URAction* Action : Actions)
	{
		if (Action && Action->ActionTag.MatchesTagExact(StopActionTag))
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	
	return false;
}

void URActionComponent::RemoveAction(FGameplayTag ActionToRemove)
{
	for (URAction* Action : Actions)
	{
		if (Action && Action->ActionTag.MatchesTagExact(ActionToRemove))
		{
			if (Action->IsRunning())
				Action->StopAction(Action->GetInstigator());

			Actions.Remove(Action);
			break;
		}
	}
}

URAction* URActionComponent::GetAction(TSubclassOf<URAction> ActionClass)
{
	for (URAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}
