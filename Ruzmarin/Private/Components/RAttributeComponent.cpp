


#include "Components/RAttributeComponent.h"

// Sets default values for this component's properties
URAttributeComponent::URAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}

URAttributeComponent* URAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

bool URAttributeComponent::ApplyAttributeChange(AActor* InstigatorActor, FGameplayTag AttributeToChange, float Delta, bool bOverrideValue /* = false */)
{
	for (int i = 0; i < Attributes.Num(); ++i)
	{
		if (Attributes[i].AttributeTag.MatchesTagExact(AttributeToChange))
		{
			// @TODO: Implement a check for blocked tags
			const float OldAttributeValue = Attributes[i].CurrentValue;
			float NewAttributeValue;
			if (bOverrideValue)
				NewAttributeValue = FMath::Clamp(Delta, 0.0f, Attributes[i].MaxValue);
			else
				NewAttributeValue = FMath::Clamp(Attributes[i].CurrentValue + Delta, 0.0f, Attributes[i].MaxValue);

			const float ActualDelta = NewAttributeValue - OldAttributeValue;

			Attributes[i].CurrentValue = NewAttributeValue;

			if (ActualDelta != 0)
				OnAttributeChanged.Broadcast(InstigatorActor, this, AttributeToChange, NewAttributeValue, Delta);

			const FGameplayTag HealthTag = FGameplayTag::RequestGameplayTag(TEXT("Attributes.Health"));

			if (ActualDelta < 0.0f && AttributeToChange == HealthTag)
			{
				// @TODO: Add kill logic here
			}

			return ActualDelta != 0;
		}
	}

	return false;
}

FAttribute URAttributeComponent::GetAttribute(FGameplayTag AttributeTag)
{
	for (FAttribute Attribute : Attributes)
	{
		if (Attribute.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Attribute;
		}
	}

	return FAttribute();
}

bool URAttributeComponent::IsAttributeMax(FGameplayTag AttributeTag)
{
	for (FAttribute Attribute : Attributes)
	{
		if (Attribute.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Attribute.CurrentValue == Attribute.MaxValue;
		}
	}

	return false;
}

float URAttributeComponent::GetAttributeValue(FGameplayTag AttributeTag)
{
	for (FAttribute Attribute : Attributes)
	{
		if (Attribute.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Attribute.CurrentValue;
		}
	}

	return 0.0f;
}

float URAttributeComponent::GetAttributeMax(FGameplayTag AttributeTag)
{
	for (FAttribute Attribute : Attributes)
	{
		if (Attribute.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Attribute.MaxValue;
		}
	}

	return 0.0f;
}

