// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wieldable.generated.h"

UCLASS()
class MINECRAFTUE_API AWieldable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWieldable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	enum ETool : uint8
	{
		Unarmed,
		Pickaxe,
		Shovel,
		Sword
	};

	enum EMaterial : uint8
	{
		None = 1,
		Wooden = 2,
		Stone = 4,
		Iron = 6,
		Golden = 8,
		Diamond = 12,
	}; 

	UPROPERTY(EditAnywhere)
	uint8 ToolType;

	UPROPERTY(EditAnywhere)
	uint8 MaterialType;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WieldableMesh;

	UPROPERTY(EditAnywhere)
	UShapeComponent* PickupTrigger;

	UPROPERTY(EditAnywhere)
	UTexture2D* PickupThumbnail;

	bool bIsActive;

	UFUNCTION()
	void OnRadiusEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Hide(bool bVis);

	void OnUsed();
};
