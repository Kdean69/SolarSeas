// Copyright © 2021 Kdean Games. All Rights Reserved.

#include "Anya_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Solar_Seas/Characters/AnyaCharacter.h"

UAnya_AnimInstance::UAnya_AnimInstance() :
	WalkSpeed(0.0f),
	bInAir(false),
	bAccelerating(false),
	MovementOffsetYaw(0.0f),
	LastMovementOffsetYaw(0.0f),
	bAiming(false),
	CharacterYaw(0.0f),
	CharacterYawLastFrame(0.0f),
	RootYawOffset(0.0f),
	Pitch(0.0f),
	bReloading(false)
{

}

void UAnya_AnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (AnyaCharacter == nullptr)
	{
		AnyaCharacter = Cast<AAnyaCharacter>(TryGetPawnOwner());
	}
	if (AnyaCharacter)
	{
		bReloading = AnyaCharacter->GetCombatState() == ECombatState::ECS_Reloading;
		// Get Character lateral speed
		FVector Velocity{ AnyaCharacter->GetVelocity() };
		Velocity.Z = 0;
		WalkSpeed = Velocity.Size();

		// Is Character in air?
		bInAir = AnyaCharacter->GetCharacterMovement()->IsFalling();

		// Is Character accelerating?
		if (AnyaCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
		{
			bAccelerating = true;
		}
		else
		{
			bAccelerating = false;
		}
		
		FRotator AimRotation = AnyaCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(AnyaCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (AnyaCharacter->GetVelocity().Size() > 0.0f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		bAiming = AnyaCharacter->GetAiming();
	}
	TurnInPlace();
}

void UAnya_AnimInstance::NativeInitializeAnimation()
{
	AnyaCharacter = Cast<AAnyaCharacter>(TryGetPawnOwner());
}

void UAnya_AnimInstance::TurnInPlace()
{
	if (AnyaCharacter == nullptr) return;

	Pitch = AnyaCharacter->GetBaseAimRotation().Pitch;

	if (WalkSpeed > 0)
	{
		// Don't turn in place, character moving
		RootYawOffset = 0.0f;
		CharacterYaw = AnyaCharacter->GetActorRotation().Yaw;
		CharacterYawLastFrame = CharacterYaw;
		RotationLastFrame = 0.0f;
		RotationCurve = 0.0f;
	}
	else
	{
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = AnyaCharacter->GetActorRotation().Yaw;

		const float YawDelta{ CharacterYaw - CharacterYawLastFrame };
		
		// Root yaw offset, updated & clamped to [-180, 180]
		RootYawOffset = 
			UKismetMathLibrary::NormalizeAxis(RootYawOffset - YawDelta);

		// 1.0 if turning,0.0 if not turning
		const float Turning{ GetCurveValue(TEXT("Turning")) };
		if (Turning > 0)
		{
			RotationLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("Rotation"));

			const float DeltaRotation{ RotationCurve = RotationLastFrame };

			// RootYawOffset > 0, turning left
			// RootYawOffset < 0, turning right
			RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

			const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
			if (ABSRootYawOffset > 90.0f)
			{
				const float YawExcess{ ABSRootYawOffset - 90.0f };
				RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
	}

	// Lets see the yaw offset
	if (GEngine) GEngine->AddOnScreenDebugMessage(
		1,
		-1,
		FColor::Blue,
		FString::Printf(TEXT("RootOffsetYaw: %f"), RootYawOffset));
}
