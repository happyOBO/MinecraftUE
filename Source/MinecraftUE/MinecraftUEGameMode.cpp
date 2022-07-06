// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftUEGameMode.h"
#include "MinecraftUEHUD.h"
#include "MinecraftUECharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMinecraftUEGameMode::AMinecraftUEGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));

	if (PlayerPawnClassFinder.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	
	// use our custom HUD class
	HUDClass = AMinecraftUEHUD::StaticClass();
}

void AMinecraftUEGameMode::BeginPlay()
{
	Super::BeginPlay();
	HUDState = EHUDState::HS_Ingame;
	ApplyHUDChanges();
}

void AMinecraftUEGameMode::ApplyHUDChanges()
{
	/* ������ HUD�� �����ϰ�, �� ���� �����Ѵ�.*/
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	/* HUD ���¸� üũ�ϰ�, �׿� �����ϴ� HUD ���� */
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

uint8 AMinecraftUEGameMode::GetHUDState()
{
	return HUDState;
}

void AMinecraftUEGameMode::SetHUDState(EHUDState state)
{
	HUDState = state;
}

void AMinecraftUEGameMode::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool AMinecraftUEGameMode::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents)
{
	/* get player controller */
	AMinecraftUECharacter* MyCharacter = Cast<AMinecraftUECharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	if (WidgetToApply != nullptr)
	{
		/* Set Mouse event and visibility */
		MyController->bShowMouseCursor = ShowMouseCursor;
		MyController->bEnableClickEvents = EnableClickEvents;

		/* ���� ���� */
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();

			return true;
		}
		else
			return false;
	}
	else
		return false;
}
