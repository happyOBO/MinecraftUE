// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class MINECRAFTUE_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();


	enum EStatus : uint8
	{
		Placed,
		Item,
	};

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 블럭이 모두 큐브 스탵틱 메시를 갖기 위해 설정
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SM_Block;

	// 깨질수 있는 최소 도구
	uint8 MinimumMaterial = 3;

	UPROPERTY(EditDefaultsOnly)
	float Resistance;

	UPROPERTY(BlueprintReadWrite)
	float BreakingStage;

	/* 블럭 상태 배치되어 있거나 아이템화 */
	UPROPERTY(BlueprintReadWrite)
	uint8 BlockStatus;

	/* 블럭을 깰 때 호출 */
	void Break();
	/* 블럭을 깨려다가 말때 호출 */
	void ResetBlock();

	/* breaking stage가 마지막 단계일 때 찐으로 깨지는 함수 호출*/
	void OnBroken(bool HasRequiredPickaxe);
};
