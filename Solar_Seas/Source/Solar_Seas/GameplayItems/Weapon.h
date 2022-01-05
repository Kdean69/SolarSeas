// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "Solar_Seas/AmmoType.h"
#include "CoreMinimal.h"
#include "Solar_Seas/GameplayItems/Item.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SMG UMETA(DisplayName = "SMG"),
	EWT_AR UMETA(DisplayName = "AR"),

	EWT_MAX UMETA(DisplayName = "Default_MAX"),
};

/**
 * 
 */
UCLASS()
class SOLAR_SEAS_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	void StopFalling();

private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	// Ammo count for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|WeaponType", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;
	// Ammp type for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Ammo", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;
	// Montage to play for equipped weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Ammo", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	// True when moving clip for reload
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons|Ammo", meta = (AllowPrivateAccess = "true"))
	bool bClipMoving;

	// Name for clip bone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Ammo", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

public:
	// Adds an impulse to weapon
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

	// Called from character class when weapon fires
	void DecrementAmmo();

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }

	void ReloadAmmo(int32 Amount);

	FORCEINLINE void SetMovingClip(bool bMove) { bClipMoving = bMove; }

	bool bClipFull();
};
