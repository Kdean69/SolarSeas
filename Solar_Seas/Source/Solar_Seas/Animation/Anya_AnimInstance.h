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

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
	class AAnyaCharacter* AnyaCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Speed", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Speed", meta = (AllowPrivateAccess = "true"))
	bool bInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Speed", meta = (AllowPrivateAccess = "true"))
	bool bAccelerating;
};
