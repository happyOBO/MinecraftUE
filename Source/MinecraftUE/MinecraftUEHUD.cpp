// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftUEHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


AMinecraftUEHUD::AMinecraftUEHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
	HUDState = EHUDState::HS_Ingame;
}


void AMinecraftUEHUD::DrawHUD()
{
	Super::DrawHUD();

	ApplyHUDChanges();
}



void AMinecraftUEHUD::ApplyHUDChanges()
{
	/* 이전의 HUD를 삭제하고, 새 것을 적용한다.*/
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	/* HUD 상태를 체크하고, 그에 상응하는 HUD 적용 */
	switch (HUDState)
	{
	case EHUDState::HS_Ingame:
	{
		ApplyHUD(IngameHUDClass, false, false);
		break;
	}
	case EHUDState::HS_Inventory:
	{
		ApplyHUD(InventoryHUDClass, true, true);
		break;
	}
	case EHUDState::HS_Craft_Menu:
	{
		ApplyHUD(CraftMenuHUDClass, true, true);
		break;
	}
	default:
	{
		ApplyHUD(IngameHUDClass, false, false);
		break;
	}
	}
}

uint8 AMinecraftUEHUD::GetHUDState()
{
	return HUDState;
}

void AMinecraftUEHUD::SetHUDState(EHUDState state)
{
	HUDState = state;
}

void AMinecraftUEHUD::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool AMinecraftUEHUD::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents)
{

	if (WidgetToApply != nullptr)
	{
		/* Set Mouse event and visibility */
		PlayerOwner->bShowMouseCursor = ShowMouseCursor;
		PlayerOwner->bEnableClickEvents = EnableClickEvents;

		/* 위젯 생성 */
		CurrentWidget = CreateWidget<UUserWidget>(PlayerOwner, WidgetToApply);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToPlayerScreen();

			return true;
		}
		else
			return false;
	}
	else
		return false;
}
