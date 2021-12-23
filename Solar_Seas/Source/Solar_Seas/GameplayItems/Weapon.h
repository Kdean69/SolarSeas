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

public:
	// Adds an impulse to weapon
	void ThrowWeapon();
};
