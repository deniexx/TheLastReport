// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static TAutoConsoleVariable<bool> CVarDisplayLogsOnScreen(TEXT("rz.DisplayLogsOnScreen"), true, TEXT("Choose whether to display logs on screen called from LogOnScreen"), ECVF_Cheat);

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 2.0f)
{
	if (CVarDisplayLogsOnScreen.GetValueOnGameThread() == false)
		return;
	
	if (!ensure(WorldContext))
	{
		return;
	}

	const UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Msg);
	}
	
}