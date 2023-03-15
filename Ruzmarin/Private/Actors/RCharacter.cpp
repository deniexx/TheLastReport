#include "Actors/RCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/RActionComponent.h"
#include "Components/RAttributeComponent.h"
#include "Components/RInteractionComponent.h"
#include "Components/RInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ARCharacter::ARCharacter()
{
	LookSensitivity = 1.0f;

	InteractionComponent = CreateDefaultSubobject<URInteractionComponent>(TEXT("InteractionComponent"));

	AttributeComponent = CreateDefaultSubobject<URAttributeComponent>(TEXT("AttributeComponent"));

	ActionComponent = CreateDefaultSubobject<URActionComponent>(TEXT("ActionComponent"));

	InventoryComponent = CreateDefaultSubobject<URInventoryComponent>(TEXT("InventoryComponent"));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}

UCameraComponent* ARCharacter::GetCameraComponent() const
{
	return CameraComponent;
}

FVector ARCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

APlayerController* ARCharacter::GetCurrentPC() const
{
	return MyController;
}

void ARCharacter::OnPosses()
{
	ARGameMode* CurrGM = GetWorld()->GetAuthGameMode<ARGameMode>();
	if (CurrGM)
	{
		LookSensitivity = CurrGM->CurrentSaveGame->CameraSensitivity;
		bPlayCameraShake = CurrGM->CurrentSaveGame->bCameraShakeEnabled;
	}

	MyController = GetController<ARPlayerController>();

	InventoryComponent->SetUp();
}

void ARCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FootstepSoundOffset < 0.0f)
	{
		if (abs(GetVelocity().X) > 10 || abs(GetVelocity().Y) > 10)
		{
			FVector EndTrace = GetActorLocation();
			EndTrace.Z = EndTrace.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 50;
			FCollisionQueryParams Params;
			Params.bReturnPhysicalMaterial = true;
			Params.AddIgnoredActor(this);
			FHitResult Hit;
		
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), EndTrace, ECollisionChannel::ECC_Visibility, Params);

			if (Hit.PhysMaterial.Get())
			{
				switch (Hit.PhysMaterial.Get()->SurfaceType)
				{
				case GRAVEL_PHYSICS_SURFACE:
					UGameplayStatics::PlaySound2D(this, GravelFSSound);
					break;
				case RUG_PHYSICS_SURFACE:
					UGameplayStatics::PlaySound2D(this, RugFSSound);
					break;
				case TILES_PHYSICS_SURFACE:
					UGameplayStatics::PlaySound2D(this, TilesFSSound);
					break;
				default:
					UGameplayStatics::PlaySound2D(this, WoodFSSound);
					break;
				}
			}

			const float Scale = GetCharacterMovement()->MaxWalkSpeed / RegularMovementSpeed;
			FootstepSoundOffset = FootstepSoundOffsetAmount / Scale;
		}
	}
	else
	{
		FootstepSoundOffset -= DeltaSeconds;
	}
}

// Called to bind functionality to input
void ARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ----------------------- BASE MOVEMENT INPUT ----------------------- //
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// ----------------------- ACTIONS INPUT ----------------------- //
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &ARCharacter::FlashlightAction);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARCharacter::ActivateSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARCharacter::StopSprint);

	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ARCharacter::ToggleInventory);

	PlayerInputComponent->BindAction("CameraToggle", IE_Pressed, this, &ARCharacter::ToggleCamera);
	PlayerInputComponent->BindAction("CameraShot", IE_Pressed, this, &ARCharacter::CameraShot);
	
	// ----------------------- LOOK INPUT ----------------------- //
	PlayerInputComponent->BindAxis("MoveForward", this, &ARCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ARCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ARCharacter::LookRight);

}

void ARCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	
	if (bPlayCameraShake && !MyController->GetIsPlayerInAnyUI() && WalkBob && Value != 0)
	{
		const float Scale = GetCharacterMovement()->MaxWalkSpeed / RegularMovementSpeed;
		MyController->PlayerCameraManager->StartCameraShake(WalkBob, Scale);
	}
	
	AddMovementInput(ControlRotation.Vector(), Value);
}

void ARCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	if (bPlayCameraShake && !MyController->GetIsPlayerInAnyUI() && WalkBob  && Value != 0)
	{
		const float Scale = GetCharacterMovement()->MaxWalkSpeed / RegularMovementSpeed;
		MyController->PlayerCameraManager->StartCameraShake(WalkBob, Scale);
	}

	const FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ARCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * LookSensitivity);
}

void ARCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void ARCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ARCharacter::ActivateSprint()
{
	ActionComponent->StartActionByTag(this, SprintActionTag);
}

void ARCharacter::StopSprint()
{
	ActionComponent->StopActionByTag(this, SprintActionTag);
}

void ARCharacter::FlashlightAction()
{
	ActionComponent->StartActionByTag(this, FlashlightActionTag);
}

void ARCharacter::ToggleInventory()
{
	InventoryComponent->ToggleInventory(this);
}

void ARCharacter::ToggleCamera()
{
	if (!ActionComponent->ActiveGameplayTags.HasTagExact(CameraMainTag))
		ActionComponent->StartActionByTag(this, CameraMainTag);
	else
		ActionComponent->StopActionByTag(this, CameraMainTag);
}

void ARCharacter::CameraShot()
{
	ActionComponent->StartActionByTag(this, CameraShotTag);
}


// ----------------------- CONSOLE COMMANDS ----------------------- //
void ARCharacter::HealSelf(float Amount)
{
	FGameplayTag HealthTag = FGameplayTag::RequestGameplayTag("Attributes.Health");
	AttributeComponent->ApplyAttributeChange(this, HealthTag, Amount);
}
