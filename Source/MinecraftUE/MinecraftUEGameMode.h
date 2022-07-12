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

public:

	/* ApplyHUDChanges 함수 호출시 변경된 HUD 상태 적용*/
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);


};



