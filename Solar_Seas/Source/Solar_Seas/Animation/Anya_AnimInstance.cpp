// Copyright © 2021 Kdean Games. All Rights Reserved.

#include "Anya_AnimInstance.h"
#include "Solar_Seas/Characters/AnyaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnya_AnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (AnyaCharacter == nullptr)
	{
		AnyaCharacter = Cast<AAnyaCharacter>(TryGetPawnOwner());
	}
	if (AnyaCharacter)
	{
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
		
	}
}

void UAnya_AnimInstance::NativeInitializeAnimation()
{
	AnyaCharacter = Cast<AAnyaCharacter>(TryGetPawnOwner());
}
