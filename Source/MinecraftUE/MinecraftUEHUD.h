// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MinecraftUEHUD.generated.h"

UCLASS()
class AMinecraftUEHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMinecraftUEHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

