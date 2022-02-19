// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Block.h"
#include "Wieldable.h"
#include "MinecraftUECharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AMinecraftUECharacter : public ACharacter
{
	GENERATED_BODY()


	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

public:
	AMinecraftUECharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_WieldedItem;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AMinecraftUEProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	/* �κ��丮 HUD�� ���� ���� ���*/
	UFUNCTION(BlueprintPure, Category = HUD)
	int32 GetCurrentInventorySlot();

	/* �κ��丮�� ������ �߰� */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddItemToInventory(AWieldable* Item);

	/* �κ��丮�� ������ ���� */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveItemToInventory(int idx);

	/* �־��� ������ ����� �������� */
	UFUNCTION(BlueprintPure, Category = Inventory)
	UTexture2D* GetThumbnailAtInventorySlot(uint8 Slot);

	/* �� �κ��丮 ���� ���� ��ȯ */
	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetNumberOfInventorySlot();


protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);


private:
	/* �κ��丮 ��Ʈ �� ���� ���� */
	const int32 NUM_OF_INVENTORY_SHORTCUT_SLOTS = 9;
	/* �κ��丮 ���� ���� */
	const int32 NUM_OF_INVENTORY_SLOTS = 36;

	/* ���� �κ��丮 ���� idx */
	int32 CurrentInventorySlot;

	/* Wield ������ ������Ʈ */
	void UpdateWieldedItem();

	/* ���� Wield ������ ��ȯ */
	AWieldable* GetCurrentlyWieldedItem();

	/* ���� Wield ������ ������ */
	void Throw();

	/* HUD ��� CraftMenu�� ���� */
	void OpenCraftMenu();

	/* �κ��丮 ���� �̵� */
	void MoveUpInventorySlot();
	void MoveDownInventorySlot();

	/* ���̸�, �� ���� */
	bool bIsBreaking;

	/* Called when hitting with a tool */
	void OnHit();
	void EndHIt();

	/* ĳ�� �ִϸ��̼� ���� */
	void PlayHitAnim();

	/* �÷��̾� �տ� ���� �ִ��� Ȯ�� */
	void CheckForBlocks();

	/* ���� ���⸦ ���� �� ȣ�� */
	void BreakBlock();

	/* �÷��̾ ���� �ٶ󺸰� �ִ� �� ���� */
	ABlock* CurrentBlock;

	/* �� üũ �Ÿ� */
	float Reach;

	/* Timer handles */
	FTimerHandle BlockBreakingHandle; // ������ ���� �ɸ��� �ð��� �ٸ� ����
	FTimerHandle HitAnimHandle;

	UPROPERTY(EditAnywhere)
	TArray<AWieldable*> Inventory;

	UPROPERTY(EditAnywhere)
	TArray<AWieldable*> Inventory;

public: 
	/* �÷��̾ ����ϴ� ���� ���� �� ��� */
	uint8 ToolType;
	uint8 MaterialType;


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

