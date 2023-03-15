


#include "Actors/RLetterInteractable.h"

#include "GameFramework/RPlayerController.h"

void ARLetterInteractable::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	ARPlayerController* PC = InstigatorPawn->GetController<ARPlayerController>();
	if (!PC) return;

	PC->OpenLetterWidget(TextOnPages);
}
