// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftCreatorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFTUE_API UCraftCreatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftCreatorComponent();

protected:


public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Craft")
	TMap<FString, TSubclassOf<class AWieldable>> CraftDict;
	
public:
	TSubclassOf<class AWieldable> GetWeidableFromID(FString ID);
};
