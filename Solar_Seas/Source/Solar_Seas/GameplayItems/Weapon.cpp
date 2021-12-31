// Copyright © 2021 Kdean Games. All Rights Reserved.


#include "Solar_Seas/GameplayItems/Weapon.h"

AWeapon::AWeapon() :
	ThrowWeaponTime(0.7f),
	bFalling(false),
	Ammo(0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keep weapon upright
	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{ 
			0.0f, GetItemMesh()->GetComponentRotation().Yaw, 0.0f };
		GetItemMesh()->SetWorldRotation(
			MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	}
}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{ 0.0f, GetItemMesh()->GetComponentRotation().Yaw, 0.0f };
	GetItemMesh()->SetWorldRotation(
		MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward{ GetItemMesh()->GetForwardVector() };
	const FVector MeshRight{ GetItemMesh()->GetRightVector() };
	// Direction weapon is thrown
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.0f, MeshForward);

	float RandomRotation{ 30.0f };
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.0f, 0.0f, 1.0f));
	ImpulseDirection *= 15'000.0f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;
	GetWorldTimerManager().SetTimer(
		ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);

}

void AWeapon::DecrementAmmo()
{
	if (Ammo - 1 <= 0)
	{
		Ammo = 0;
	}
	else
	{
		--Ammo;
	}
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
}
