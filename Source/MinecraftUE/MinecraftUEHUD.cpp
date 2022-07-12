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

	//// Draw very simple crosshair

	//// find center of the Canvas
	//const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	//const FVector2D CrosshairDrawPosition( (Center.X),
	//									   (Center.Y + 20.0f));

	//// draw the crosshair
	//FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	//TileItem.BlendMode = SE_BLEND_Translucent;
	//Canvas->DrawItem( TileItem );

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
	}	case EHUDState::HS_Craft_Menu:
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
