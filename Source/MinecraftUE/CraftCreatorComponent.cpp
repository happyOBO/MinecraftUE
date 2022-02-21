// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftCreatorComponent.h"

// Sets default values for this component's properties
UCraftCreatorComponent::UCraftCreatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


TSubclassOf<class AWieldable> UCraftCreatorComponent::GetWeidableFromID(FString Input)
{
	if (CraftDict.Contains(Input))
		return CraftDict[Input];
	else
		return NULL;
}