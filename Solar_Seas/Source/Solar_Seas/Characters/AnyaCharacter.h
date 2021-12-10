// Copyright © 2021 Kdean Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnyaCharacter.generated.h"

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
	
	void FireWeapon();

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

	// In deg/sec. Othering scaling may effect final rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Player", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Player", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// Weapon sounds & effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* WeaponFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzelFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|AnimMontage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* WeaponFireMontage;

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetThirdPersonCamera() const { return ThirdPersonCamera; }

};