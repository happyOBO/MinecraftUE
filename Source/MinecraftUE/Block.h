// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "Block.generated.h"

UCLASS()
class MINECRAFTUE_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* ���� �� �� ȣ�� */
	void Break();
	/* ���� �����ٰ� ���� ȣ�� */
	void ResetBlock();

	/* breaking stage�� ������ �ܰ��� �� ������ ������ �Լ� ȣ��*/
	// UFUNCTION(Server, Reliable)
	void OnBroken(bool HasRequiredPickaxe);

	UFUNCTION()
	void OnRep_Breaking();

	UFUNCTION()
	void SetBreakBlockMaterial();


public:
	// ���� ��� ť�� ����ƽ �޽ø� ���� ���� ����
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SM_Block;

	// ������ �ִ� �ּ� ����
	uint8 MinimumMaterial = 3;

	UPROPERTY(EditDefaultsOnly)
	float Resistance;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing = OnRep_Breaking)
	float BreakingStage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprint Wieldable Block", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class AActor> WieldableBlock;


};
