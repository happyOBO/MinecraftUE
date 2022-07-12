// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MincraftUEPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTUE_API AMincraftUEPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
