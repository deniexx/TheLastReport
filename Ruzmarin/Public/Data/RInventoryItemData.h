

#pragma once

#include "CoreMinimal.h"
#include "Actions/RAction.h"
#include "Engine/DataAsset.h"
#include "RInventoryItemData.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URInventoryItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	TSubclassOf<URAction> ItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	FVector DefaultItemScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	FVector BoxScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	FVector BoxLocation;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Items", GetFName());
	}
	
};
