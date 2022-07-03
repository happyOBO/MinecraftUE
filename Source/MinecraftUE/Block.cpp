// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"


// Sets default values
ABlock::ABlock()
{
	bReplicates = true;
	SM_Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	Resistance = 20.0f;
	BreakingStage = 0.0f;
	MinimumMaterial = 0;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);

}

void ABlock::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlock, BreakingStage);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABlock::Break()
{
	if (BreakingStage == 5.0f)
	{
		OnBroken(true);
	}

	++BreakingStage;
	// SetBreakBlockMaterial();

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
	UE_LOG(LogTemp, Warning, TEXT("OnBroken_Implementation"));
	GetWorld()->SpawnActor<AActor>(WieldableBlock, GetActorLocation(), GetActorRotation());

	Destroy();
	

}

void ABlock::OnRep_Breaking()
{
	if (GetLocalRole() == ROLE_Authority)
		return;

	SetBreakBlockMaterial();

	UE_LOG(LogTemp, Warning, TEXT("OnRep_Breaking %f"), BreakingStage);
	if (BreakingStage == 5.0f)
	{
		OnBroken(true);
	}

}

void ABlock::SetBreakBlockMaterial()
{
	float CrackingValue = 1.0f - (BreakingStage / 5.0f); // 1 0.9 ... 0.1
	UMaterialInstanceDynamic* MatInstance = SM_Block->CreateDynamicMaterialInstance(0);

	if (MatInstance != nullptr)
	{
		MatInstance->SetScalarParameterValue(FName("CrackingValue"), CrackingValue);
	}


}


