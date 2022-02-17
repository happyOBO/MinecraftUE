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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// ���� ��� ť�� ����ƽ �޽ø� ���� ���� ����
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SM_Block;

	// ������ �ִ� �ּ� ����
	uint8 MinimumMaterial = 3;

	UPROPERTY(EditDefaultsOnly)
	float Resistance;

	UPROPERTY(BlueprintReadWrite)
	float BreakingStage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Wieldable Block", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class AActor> WieldableBlock;

	/* ���� �� �� ȣ�� */
	void Break();
	/* ���� �����ٰ� ���� ȣ�� */
	void ResetBlock();

	/* breaking stage�� ������ �ܰ��� �� ������ ������ �Լ� ȣ��*/
	void OnBroken(bool HasRequiredPickaxe);
};
