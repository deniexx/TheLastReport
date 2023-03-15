

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RUZMARIN_API URSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	float MasterVolume = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	float MusicVolume = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	float SFXVolume = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	float VoiceVolume = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	bool bSubtitles = true;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	bool bFirstLaunch = true;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	float CameraSensitivity = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	bool bCameraShakeEnabled = true;
};
