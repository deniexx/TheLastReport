

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void ShowTextBlock();
	
protected:

	UPROPERTY(BlueprintReadWrite)
	bool bCanClose;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( BindWidget ))
	class UCircularThrobber* CircularThrobber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( BindWidget ))
	class UTextBlock* TextBlock;
};
