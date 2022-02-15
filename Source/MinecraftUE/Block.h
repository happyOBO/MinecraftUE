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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 블럭이 모두 큐브 스탵틱 메시를 갖기 위해 설정
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SM_Block;

};
