// Fill out your copyright notice in the Description page of Project Settings.

#include "Wieldable.h"
#include "Components/BoxComponent.h"
#include "MinecraftUECharacter.h"


// Sets default values
AWieldable::AWieldable()
{
	// 매 프레임마다 tick 함수를 실행할건지 유무
	PrimaryActorTick.bCanEverTick = true;

	WieldableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WieldableMesh"));

	PickupTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTrigger"));

	PickupTrigger->SetGenerateOverlapEvents(true);
	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWieldable::OnRadiusEnter);
	PickupTrigger->SetupAttachment(WieldableMesh);

	MaterialType = EMaterial::None;
	ToolType = ETool::Unarmed;

	bIsActive = true;
}

// Called when the game starts or when spawned
void AWieldable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWieldable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rotation = WieldableMesh->GetComponentRotation();
	rotation.Yaw += 1.0f;
	// 1도씩 회전
	WieldableMesh->SetRelativeRotation(rotation);
}

void AWieldable::OnRadiusEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMinecraftUECharacter* Character = Cast<AMinecraftUECharacter>(OtherActor);
	if (Character && bIsActive)
	{
		Character->AddItemToInventory(this);
		Hide(true);
	}
}



void AWieldable::Hide(bool bVis)
{
	WieldableMesh->SetVisibility(!bVis);
	bIsActive = !bVis;
}

void AWieldable::OnUsed()
{
	Destroy();
}