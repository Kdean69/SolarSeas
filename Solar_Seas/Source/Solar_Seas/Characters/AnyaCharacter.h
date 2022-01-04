// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Solar_Seas/AmmoType.h"
#include "AnyaCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "Fire Timer In Progress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_NAX UMETA(DisplayName = "Default MAX"),
};

UCLASS()
class SOLAR_SEAS_API AAnyaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnyaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	/**
	* Called via input to turn at given rate
	* @param Rate	Normalized rate. 1.0 = 100% of rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to look up/down at given rate
	* @param Rate	Normalized rate. 1.0 = 100% of rate
	*/
	void LookUpAtRate(float Rate);
	
	/**
	* Rotate controller based on mouse X movement
	* @param Value	Input value from mouse
	*/
	void Turn(float Value);
	/**
	* Rotate controller based on mouse Y movement
	* @param Value	Input value from mouse
	*/
	void LookUp(float Value);

	void FireWeapon();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);


	// Set bAiming when trigger/button press/release
	void CharacterAimingTriggered();
	void CharacterAimingReleased();

	void CharacterAimZoom(float DeltaTime);

	// Set BaseTurnRate & BaseLookUpRate based on Aiming
	void SetLookRates();

	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	// Line trace for items under crosshairs
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	// Trace for items if over lap item count > 0
	void TraceForItems();

	// Spawns default weapon and equips it
	class AWeapon* SpawnDefaultWeapon();
	// Attached weapon to the mesh
	void EquipWeapon(class AWeapon* WeaponToEquip);
	// Drop to ground
	void DropWeapon();

	void SelectButtonPressed();

	void SelectButtonReleased();

	// Drops current equipped weapon and equips TraceHitItem
	void SwapWeapon(AWeapon* WeaponToSwap);

	// Init with starting values
	void InitializeAmmoMap();

	bool bWeaponHasAmmo();

	// Fire weapon functions
	void PlayWeaponFireSound();
	void ShootBullet();
	void PlayWeaponFireMontage();
	// Bound to R key and gamepad face btn Left
	void ReloadButtonPressed();
	void ReloadWeapon();

	// Checks to see if character has ammo for equipped weapon type
	bool bCarryingAmmo();

	// Called from anim blueprint with Grab Clip notify
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	// Called from anim blueprint with Release Clip notify
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// Camera boom to postition the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;
	
	// The default camera view
	float CameraDefaultFOV;
	// Field of view for the current frame
	float CameraCurrentFOV;
	// View when aiming weapons
	float CameraAimingFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Camera", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

	// In deg/sec. Othering scaling may effect final rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Player", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Player", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
	// Aiming turn and lookup rates
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	/**
	*		Mouse scaled rates
	*/
	// Scaled mouse look sensitivity when NOT aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, 
		Category = "Camera|Aiming Rate", 
		meta = (AllowPrivateAccess = "true"), 
		meta= (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	// Scaled mouse look sensitivity when NOT aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;
	// Scaled mouse look sensitivity when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;
	
	// Scaled mouse look sensitivity when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"),
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Aiming Rate", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	// Weapon sounds & effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* WeaponFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;
	// Spwaned on bullet impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|AnimMontage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* WeaponFireMontage;
	// Bullet smoke trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* BeamParticles;

	// Cross-hairs
	// // True when aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Cross-hairs", meta = (ALlowPrivateAccess = "true"))
	bool bAiming;

	bool bFiringBullet;

	// Spread of the cross-hairs
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Cross-hairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	// Velocity component for cross0hair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Cross-hairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;
	// In air component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Cross-hairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;
	// Aim for cross0hair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Cross-hairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;
	// Shooting component for cross-hair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Cross-hairs", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;
	
	float ShootTimeDuration;

	FTimerHandle CrosshairShootTimer;
	
	// Left mouse or right trigger
	bool bFireButtonPressed;
	// True when can fire
	bool bShouldFire;
	// Auto-Gun-Fire rate
	float AutomaticFireRate;
	// Set time between gunshots
	FTimerHandle AutoFireTimer;

	// True if we want line trace per frame
	bool bTraceForItems;
	// Number of overlapped items
	int8 OverlappedItemCount;
	
	// The AItem hit last frame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLastFrame;

	// Current equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;
	// Set in Blueprints for default weapon class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	// Item currently hit by TraceForItems (could be NULL)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	// Distance outward from camera for the interp destination
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float CameraInterpDistance;
	// Distance upward from camera for the interp destination
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation;
	
	// Map to keep track of different ammo types
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;
	
	// 9mm starting ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo;
	// AR starting ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 StartingSMGAmmo;

	// Can only fire or reload if unoccupied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|ReloadMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	// Transform of clip when grab clip
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|ReloadMontage", meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;
	// Scene component to attach to charaters hand during reload.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|ReloadMontage", meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* 
		GetThirdPersonCamera() const { return ThirdPersonCamera; }

	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	// Adds/Subtracts overlapped item count & Updates bTraceForItems
	void IncrementOverlappedItemCount(int8 Amount);

	FVector GetCameraInterpLocation();

	void GetPickupItem(AItem* Item);
};