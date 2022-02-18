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

	/* 현재 HUD 상태에 맞게 HUD 변경  */
	void ApplyHUDChanges();

	/* HUDState 상태 반환 */
	uint8 GetHUDState();

	/* HUDState 상태 설정 */
	void SetHUDState(EHUDState state);

	/* ApplyHUDChanges 함수 호출시 변경된 HUD 상태 적용*/
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);

	/* 화면에 HUD 적용한다 (성공/실패 반환) */
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents);

protected:
	/* 현재 HUD 상태 */
	uint8 HUDState;

	/* 인게임 내에 보여지는 HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> IngameHUDClass;

	/* 인벤트리 HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> InventoryHUDClass;
	
	/* Craft Menu HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Widgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> CraftMenuHUDClass;

	/* 현재 화면에 보여지고 있는 HUD */
	class UUserWidget* CurrentWidget;

};



