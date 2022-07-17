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
	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Craft_Menu,

	};

public:
	AMinecraftUEHUD();

	void ApplyHUDChanges();

	/* HUDState ���� ��ȯ */
	uint8 GetHUDState();

	/* HUDState ���� ���� */
	void SetHUDState(EHUDState state);

protected:

	/* ȭ�鿡 HUD �����Ѵ� (����/���� ��ȯ) */
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents);

	/* ApplyHUDChanges �Լ� ȣ��� ����� HUD ���� ����*/
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);


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


protected:
	/* ���� ȭ�鿡 �������� �ִ� HUD */
	class UUserWidget* CurrentWidget;

};

