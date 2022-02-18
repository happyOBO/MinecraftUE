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

	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Craft_Menu,

	};

	/* ���� HUD ���¿� �°� HUD ����  */
	void ApplyHUDChanges();

	/* HUDState ���� ��ȯ */
	uint8 GetHUDState();

	/* HUDState ���� ���� */
	void SetHUDState(EHUDState state);

	/* ApplyHUDChanges �Լ� ȣ��� ����� HUD ���� ����*/
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);

	/* ȭ�鿡 HUD �����Ѵ� (����/���� ��ȯ) */
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents);

protected:
	/* ���� HUD ���� */
	uint8 HUDState;

	/* �ΰ��� ���� �������� HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> IngameHUDClass;

	/* �κ�Ʈ�� HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> InventoryHUDClass;
	
	/* Craft Menu HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> CraftMenuHUDClass;

	/* ���� ȭ�鿡 �������� �ִ� HUD */
	class UUserWidget* CurrentWidget;

};



