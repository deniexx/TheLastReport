#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "RCharacter.generated.h"

#define WOOD_PHYSICS_SURFACE EPhysicalSurface::SurfaceType1
#define GRAVEL_PHYSICS_SURFACE EPhysicalSurface::SurfaceType2
#define RUG_PHYSICS_SURFACE EPhysicalSurface::SurfaceType3
#define TILES_PHYSICS_SURFACE EPhysicalSurface::SurfaceType4

UCLASS()
class RUZMARIN_API ARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARCharacter();

	class UCameraComponent* GetCameraComponent() const;

	void OnPosses();

	APlayerController* GetCurrentPC() const;

protected:

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void LookUp(float Value);

	UFUNCTION()
	void LookRight(float Value);

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void ActivateSprint();

	UFUNCTION()
	void StopSprint();
	
	UFUNCTION()
	void FlashlightAction();

	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void ToggleCamera();
	
	UFUNCTION()
	void CameraShot();

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FGameplayTag SprintActionTag;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FGameplayTag FlashlightActionTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FGameplayTag CameraMainTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FGameplayTag CameraShotTag;
	
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Input")
	float LookSensitivity;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class URInteractionComponent* InteractionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class URAttributeComponent* AttributeComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class URActionComponent* ActionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class URInventoryComponent* InventoryComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> WalkBob;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RegularMovementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	float FootstepSoundOffsetAmount = 0.2f;
	
	UPROPERTY()
	float FootstepSoundOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* WoodFSSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* GravelFSSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* RugFSSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* TilesFSSound;

	UPROPERTY()
	bool bPlayCameraShake;

	UPROPERTY()
	class ARPlayerController* MyController;
	
	virtual FVector GetPawnViewLocation() const override;
	
	virtual void Tick(float DeltaSeconds) override;


	// ----------------------- CONSOLE COMMANDS ----------------------- //
	
	UFUNCTION(Exec)
	void HealSelf(float Amount);
	
public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
