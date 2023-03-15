

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	USceneComponent* AttachedComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class USizeBox* ParentSizeBox;
};
