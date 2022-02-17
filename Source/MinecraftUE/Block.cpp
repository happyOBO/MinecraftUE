// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"

// Sets default values
ABlock::ABlock()
{
	// 매 프레임마다 tick 함수를 실행할건지 유무
	PrimaryActorTick.bCanEverTick = true;
	SM_Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	Resistance = 20.0f;
	BreakingStage = 0.0f;
	MinimumMaterial = 0;
	BlockStatus = EStatus::Placed;
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BlockStatus == EStatus::Item)
	{
		FRotator rotation = SM_Block->GetComponentRotation();
		rotation.Yaw += 1.0f;
		// 1도씩 회전
		SM_Block->SetRelativeRotation(rotation);
	}
}

void ABlock::Break()
{
	++BreakingStage;
	float CrackingValue = 1.0f - (BreakingStage / 5.0f); // 1 0.9 ... 0.1

	UMaterialInstanceDynamic* MatInstance = SM_Block->CreateDynamicMaterialInstance(0);

	if (MatInstance != nullptr)
	{
		MatInstance->SetScalarParameterValue(FName("CrackingValue"), CrackingValue);
	}
	if (BreakingStage == 5.0f)
	{
		OnBroken(true);
	}
}

void ABlock::ResetBlock()
{
	BreakingStage = 0.0f;
	UMaterialInstanceDynamic* MatInstance = SM_Block->CreateDynamicMaterialInstance(0);

	if (MatInstance != nullptr)
	{
		MatInstance->SetScalarParameterValue(FName("CrackingValue"), 1.0f);
	}
}

void ABlock::OnBroken(bool HasRequiredPickaxe)
{
	BlockStatus = EStatus::Item;
	FVector scale = FVector(0.5f, 0.5f, 0.5f);
	SM_Block->SetRelativeScale3D(scale);
	// Destroy();
}

