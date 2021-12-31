// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Solar_Seas/GameplayItems/Item.h"
#include "Weapon.generated.h"

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

public:
	// Adds an impulse to weapon
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	// Called from character class when weapon fires
	void DecrementAmmo();
};
