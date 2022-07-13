// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinecraftUEGameMode.generated.h"

UCLASS(minimalapi)
class AMinecraftUEGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMinecraftUEGameMode();

protected:
	virtual void BeginPlay() override;

};



