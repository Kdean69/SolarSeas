// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anya_AnimInstance.generated.h"

UCLASS()
class SOLAR_SEAS_API UAnya_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnya_AnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:
	// Turn in place var
	void TurnInPlace();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anya Movement", meta = (AllowPrivateAccess = "true"))
	class AAnyaCharacter* AnyaCharacter;

	// Walking speed of character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anya Movement|Walk Speed", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anya Movement|In Air", meta = (AllowPrivateAccess = "true"))
	bool bInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anya Movement|Is Accelerating", meta = (AllowPrivateAccess = "true"))
	bool bAccelerating;

	// Offset yaw used for strafing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Straf", meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Last Offset Yaw", meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Aiming", meta = (AllowPrivateAccess = "true"))
	bool bAiming;
	// Yaw of character this frame
	float CharacterYaw;
	// Yaw of character previous frame
	float CharacterYawLastFrame;
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Turn In Place", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	// Rotation curve this frame
	float RotationCurve;
	// Rotation curve last frame
	float RotationLastFrame;

	// Pitch of aim rotation for Aim Offset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Turn In Place", meta = (AllowPrivateAccess = "true"))
	float Pitch;
	// True while reload, prevent Aim Offset during reloading weapon.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Turn In Place", meta = (AllowPrivateAccess = "true"))
	bool bReloading;
};
