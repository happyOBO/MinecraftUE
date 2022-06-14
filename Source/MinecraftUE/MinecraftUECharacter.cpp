// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftUECharacter.h"
#include "MinecraftUEProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "MinecraftUEGameMode.h"
#include "CraftCreatorComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMinecraftUECharacter

AMinecraftUECharacter::AMinecraftUECharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_WieldedItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_WieldedItem"));
	FP_WieldedItem->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_WieldedItem->bCastDynamicShadow = false;
	FP_WieldedItem->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_WieldedItem->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_WieldedItem);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Create a CraftCreator
	CraftCreator = CreateDefaultSubobject<UCraftCreatorComponent>(TEXT("CRAFTCREATOR"));


	Reach = 300.0f;
	Inventory.SetNum(NUM_OF_INVENTORY_SLOTS);
	CraftInventory.SetNum(NUM_OF_CRAFT_INVENTORY_SLOTS);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;


}

void AMinecraftUECharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_WieldedItem->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}


	

}

void AMinecraftUECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForBlocks();

	//auto GameMode = Cast<AMinecraftUEGameMode>(GetWorld()->GetAuthGameMode());
	//if (GameMode->GetHUDState() == AMinecraftUEGameMode::EHUDState::HS_Craft_Menu)
	UpdatePossibleCraftWeildable();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMinecraftUECharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("InventoryUp", IE_Pressed, this, &AMinecraftUECharacter::MoveUpInventorySlot);
	InputComponent->BindAction("InventoryDown", IE_Pressed, this, &AMinecraftUECharacter::MoveDownInventorySlot);
	
	PlayerInputComponent->BindAction("CraftMenu", IE_Pressed, this, &AMinecraftUECharacter::OpenCraftMenu);
	
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AMinecraftUECharacter::Throw);

	// Enable touchscreen input, Bind fire event
	if (EnableTouchscreenMovement(InputComponent) == false)
	{
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMinecraftUECharacter::OnHit);
		PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMinecraftUECharacter::EndHIt);

	}

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMinecraftUECharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMinecraftUECharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMinecraftUECharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMinecraftUECharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMinecraftUECharacter::LookUpAtRate);
}

int32 AMinecraftUECharacter::GetCurrentInventorySlot()
{
	return CurrentInventorySlot;
}

bool AMinecraftUECharacter::AddItemToInventory(AWieldable* Item)
{

	if (Item != NULL)
	{

		const int32 AvailableSlot = Inventory.Find(nullptr);
		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			UE_LOG(LogTemp, Warning, TEXT("Inventory %d"), AvailableSlot);
			return true;
		}
		else
			return false;
	}
	return false;
}

bool AMinecraftUECharacter::RemoveItemToInventory(int idx)
{
	if (NUM_OF_INVENTORY_SLOTS <= idx || idx < 0)
		return false;

	Inventory[idx] = nullptr;
	return true;
}



UTexture2D* AMinecraftUECharacter::GetThumbnailAtInventorySlot(uint8 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupThumbnail;
	}

	else
		return nullptr;
}

UTexture2D* AMinecraftUECharacter::GetThumbnailAtCraftInventorySlot(uint8 Slot)
{
	if (CraftInventory[Slot] != NULL)
	{
		return CraftInventory[Slot]->PickupThumbnail;
	}

	else
		return nullptr;
}

UTexture2D* AMinecraftUECharacter::GetThumbnailAtPossibleCraftWeildable()
{
	if (PossibleWieldable == nullptr)
		return nullptr;
	auto texture = PossibleWieldable.GetDefaultObject();
	return texture->PickupThumbnail;
}


int32 AMinecraftUECharacter::GetNumberOfInventorySlot()
{
	return NUM_OF_INVENTORY_SLOTS;
}

int32 AMinecraftUECharacter::GetNumberOfCraftInventorySlot()
{
	return NUM_OF_CRAFT_INVENTORY_SLOTS;
}

bool AMinecraftUECharacter::MoveToCraftInventory(uint8 fromInventoryIdx, uint8 toCraftInventoryIdx)
{
	if (toCraftInventoryIdx < 0 || NUM_OF_CRAFT_INVENTORY_SLOTS <= toCraftInventoryIdx)
		return false;
	if (NUM_OF_INVENTORY_SLOTS <= fromInventoryIdx || fromInventoryIdx < 0)
		return false;
	CraftInventory[toCraftInventoryIdx] = Inventory[fromInventoryIdx];
	Inventory[fromInventoryIdx] = nullptr;
	return true;
}

bool AMinecraftUECharacter::MoveToInventory(uint8 fromCraftInventoryIdx)
{
	if (fromCraftInventoryIdx < 0 || NUM_OF_CRAFT_INVENTORY_SLOTS <= fromCraftInventoryIdx)
		return false;

	const int32 AvailableSlot = Inventory.Find(nullptr);
	if (AvailableSlot != INDEX_NONE)
	{
		Inventory[AvailableSlot] = CraftInventory[fromCraftInventoryIdx];
		CraftInventory[fromCraftInventoryIdx] = nullptr;
		return true;
	}
	else
		return false;

}

bool AMinecraftUECharacter::GetCraftWeidable(uint8 toInventoryIdx)
{

	if (NUM_OF_INVENTORY_SLOTS <= toInventoryIdx || toInventoryIdx < 0)
		return false;

	// �ٷ� ���� ���ڸ��� ������ �Ǿ �κ��丮�� �ΰ��� �߰��Ǿ� ������ ���� ��Ŵ(���� �ʿ�)
	FVector location = GetActorLocation();
	location.X += 100.0f;
	auto CraftItem = Cast<AWieldable>(GetWorld()->SpawnActor<AActor>(PossibleWieldable, location, GetActorRotation()));
	Inventory[toInventoryIdx] = CraftItem;
	CraftItem->Hide(true);

	PossibleWieldable = nullptr;
	for (int i = 0; i < NUM_OF_CRAFT_INVENTORY_SLOTS; i++)
	{
		CraftInventory[i] = nullptr;
	}

	return true;
}

void AMinecraftUECharacter::OnFire()
{

}

void AMinecraftUECharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMinecraftUECharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AMinecraftUECharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}


void AMinecraftUECharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
	
}

void AMinecraftUECharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMinecraftUECharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMinecraftUECharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMinecraftUECharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMinecraftUECharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMinecraftUECharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMinecraftUECharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void AMinecraftUECharacter::UpdateWieldedItem()
{
	if (Inventory[CurrentInventorySlot] != nullptr)
	{
		FP_WieldedItem->SetSkeletalMesh(Inventory[CurrentInventorySlot]->WieldableMesh->SkeletalMesh);
		ToolType = Inventory[CurrentInventorySlot]->ToolType;
		MaterialType = Inventory[CurrentInventorySlot]->MaterialType;

	}
	else
	{
		FP_WieldedItem->SetSkeletalMesh(NULL);
		ToolType = AWieldable::ETool::Unarmed;
		MaterialType = AWieldable::EMaterial::None;
	}
}

AWieldable* AMinecraftUECharacter::GetCurrentlyWieldedItem()
{
	return Inventory[CurrentInventorySlot] != NULL ? Inventory[CurrentInventorySlot] : nullptr;
}

void AMinecraftUECharacter::Throw()
{
	AWieldable* ItemToThrow = GetCurrentlyWieldedItem();

	// ���� ���� ��ġ ���
	FHitResult LinetraceHit;

	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * Reach) + StartTrace;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(LinetraceHit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, CQP);

	FVector DropLocation = EndTrace;

	if (LinetraceHit.GetActor() != NULL)
	{
		DropLocation = (LinetraceHit.ImpactPoint + 20.0f);
	}
	if (ItemToThrow != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			ItemToThrow->SetActorLocationAndRotation(DropLocation, FRotator::ZeroRotator);
			ItemToThrow->Hide(false);
			Inventory[CurrentInventorySlot] = nullptr;
		}
	}

	UpdateWieldedItem();
}

void AMinecraftUECharacter::OpenCraftMenu()
{
	auto GameMode = Cast<AMinecraftUEGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetHUDState() == AMinecraftUEGameMode::EHUDState::HS_Craft_Menu)
	{
		for(int i =0; i < NUM_OF_CRAFT_INVENTORY_SLOTS; i++)
		{
			if (CraftInventory[i] == nullptr) continue;
			MoveToInventory(i);
		}
		GameMode->SetHUDState(AMinecraftUEGameMode::EHUDState::HS_Ingame);
	}
	else
		GameMode->SetHUDState(AMinecraftUEGameMode::EHUDState::HS_Craft_Menu);

	GameMode->ApplyHUDChanges();
}

void AMinecraftUECharacter::MoveUpInventorySlot()
{
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot + 1) % NUM_OF_INVENTORY_SHORTCUT_SLOTS);
	UpdateWieldedItem();
}

void AMinecraftUECharacter::MoveDownInventorySlot()
{
	if (CurrentInventorySlot == 0)
		CurrentInventorySlot = NUM_OF_INVENTORY_SHORTCUT_SLOTS - 1;
	else
		CurrentInventorySlot = FMath::Abs((CurrentInventorySlot - 1) % NUM_OF_INVENTORY_SHORTCUT_SLOTS);
	UpdateWieldedItem();
}

void AMinecraftUECharacter::OnHit()
{
	PlayHitAnim();

	if (CurrentBlock != nullptr)
	{
		bIsBreaking = true;
		float TimerBetweenBreaks = ((CurrentBlock->Resistance) / 100.0f) / 2; // 2, CurrenctTool->Power
		// Ÿ�̸� ���� , 1. �� ������ �ð� 2. �÷��̾� ���� �ִϸ��̼� ���� �ð�
		GetWorld()->GetTimerManager().SetTimer(BlockBreakingHandle, this, &AMinecraftUECharacter::BreakBlock, TimerBetweenBreaks, true);
		GetWorld()->GetTimerManager().SetTimer(HitAnimHandle, this, &AMinecraftUECharacter::PlayHitAnim, 0.4f, true); // ������ �Ҷ� Ŭ�� ���������� ������ ������ 0.4�ʷ� ����
	
	}
}

void AMinecraftUECharacter::EndHIt()
{
	GetWorld()->GetTimerManager().ClearTimer(BlockBreakingHandle);
	GetWorld()->GetTimerManager().ClearTimer(HitAnimHandle);

	bIsBreaking = false;

	if (CurrentBlock != nullptr)
	{
		CurrentBlock->ResetBlock();
	}

}

void AMinecraftUECharacter::PlayHitAnim()
{
	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AMinecraftUECharacter::CheckForBlocks()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		FHitResult LinetraceHit;

		FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
		FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * Reach) + StartTrace;

		FCollisionQueryParams CQP;
		CQP.AddIgnoredActor(this); // �÷��̾�� raycast�� ����
	
		GetWorld()->LineTraceSingleByChannel(LinetraceHit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, CQP);

		ABlock* PotentialBlock = Cast<ABlock>(LinetraceHit.GetActor());
	
		if (PotentialBlock != CurrentBlock && CurrentBlock != nullptr) // ���� ������ ���� �ٸ� ������ �ü��� �̵��Ǿ��� ��
		{
			CurrentBlock->ResetBlock();
		}
		if (PotentialBlock == NULL)
		{
			CurrentBlock = nullptr;
			return;
		}
		else
		{
			if(CurrentBlock != nullptr && !bIsBreaking)
				CurrentBlock->ResetBlock();
			CurrentBlock = PotentialBlock;
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *CurrentBlock->GetName());
		}
	}
}

void AMinecraftUECharacter::BreakBlock()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		if (bIsBreaking && CurrentBlock != nullptr && !CurrentBlock->IsPendingKill())
		{
			ServerBreakBlock(CurrentBlock);

		}

	}
		
}

void AMinecraftUECharacter::UpdatePossibleCraftWeildable()
{
	FString CraftInputs;
	for (int i = 0; i < NUM_OF_CRAFT_INVENTORY_SLOTS; i++)
	{
		if (CraftInventory[i] == nullptr)
			CraftInputs += TEXT("00");
		else
		{
			CraftInputs += FString::FromInt(CraftInventory[i]->ToolType);
			CraftInputs += FString::FromInt(CraftInventory[i]->MaterialType);
		}
	}
	auto CraftWieldable = CraftCreator->GetWeidableFromID(CraftInputs);
	if (CraftWieldable == NULL)
		PossibleWieldable = nullptr;
	else
		PossibleWieldable = CraftWieldable;
	

}

void AMinecraftUECharacter::ServerBreakBlock_Implementation(ABlock* block)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		block->Break();
	}
}
