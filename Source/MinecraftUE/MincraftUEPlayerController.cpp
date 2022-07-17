// Fill out your copyright notice in the Description page of Project Settings.


#include "MincraftUEPlayerController.h"
#include "MinecraftUEHUD.h"

void AMincraftUEPlayerController::BeginPlay()
{
	AHUD* CurrentHUD = GetHUD();
	if (CurrentHUD)
	{
		AMinecraftUEHUD* MyMinecraftUEHUD = Cast<AMinecraftUEHUD>(CurrentHUD);
		MyMinecraftUEHUD->SetHUDState(AMinecraftUEHUD::EHUDState::HS_Ingame);
		MyMinecraftUEHUD->ApplyHUDChanges();
	}

}