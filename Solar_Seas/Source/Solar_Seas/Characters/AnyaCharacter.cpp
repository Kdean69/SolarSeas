// Copyright © 2021 Kdean Games. All Rights Reserved.


#include "AnyaCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAnyaCharacter::AAnyaCharacter() :
	// Base rates
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	// Turn rates for aim/not aiming
	HipTurnRate(90.0f),
	HipLookUpRate(90.0f),
	AimingTurnRate(20.0f),
	AimingLookUpRate(20.0f),
	// Mouse look sensitivity scale factors
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.2f),
	MouseAimingLookUpRate(0.2f),
	// Camera field of Views
	CameraDefaultFOV(0.0f),	// Set in BeginPlay
	CameraAimingFOV(28.0f),
	CameraCurrentFOV(0.0f),
	ZoomInterpSpeed(20.0f),
	// True when aiming a weapon
	bAiming(false),
	// Crosshair spread factors
	CrosshairSpreadMultiplier(0.0f),
	CrosshairVelocityFactor(0.0f),
	CrosshairInAirFactor(0.0f),
	CrosshairAimFactor(0.0f),
	CrosshairShootingFactor(0.0f),
	// Bullet fire timer
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	// AutoGun-Fire variables
	AutomaticFireRate(0.1f),
	bShouldFire(true),
	bFireButtonPressed(false)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up camera and boom to character
	// Create Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 310.0f;	// Follow length
	CameraBoom->bUsePawnControlRotation = true;	// Rotate boom based on controller
	CameraBoom->SocketOffset = FVector(0.0f, 50.0f, 70.0f);

	// Create Camera
	ThirdPersonCamera = 
		CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	
	ThirdPersonCamera->SetupAttachment(
		CameraBoom, 
		USpringArmComponent::SocketName); // Attach to CameraBoom
	
	ThirdPersonCamera->bUsePawnControlRotation = true;	// Camera does not rotate relative to Boom

	// Turn off so that camera rotates around character
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	// Config character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AAnyaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ThirdPersonCamera)
	{
		CameraDefaultFOV = GetThirdPersonCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;

	}
	
}

void AAnyaCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find out forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		AddMovementInput(Direction, Value);
	}
}

void AAnyaCharacter::MoveRight(float Value)
{
	// Find out right direction
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AAnyaCharacter::TurnAtRate(float Rate)
{
	// Calc delta for this frame
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAnyaCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAnyaCharacter::Turn(float Value)
{	
	float TurnScaleFactor{};
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AAnyaCharacter::LookUp(float Value)
{
	float LookUpScaleFactor{};
	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AAnyaCharacter::FireWeapon()
{
	if (WeaponFireSound)
	{
		UGameplayStatics::PlaySound2D(this, WeaponFireSound);
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), 
				MuzzleFlash, 
				SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);

		if (bBeamEnd)
		{
			// Spawn impact particles after BeamEndPoint updating
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					BeamEnd);
			}

			if (BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					BeamParticles,
					SocketTransform);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}
		}

	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && WeaponFireMontage)
	{
		AnimInstance->Montage_Play(WeaponFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	// Start bullet fire timer for crosshairs
	StartCrosshairBulletFire();
}

bool AAnyaCharacter::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation, 
	FVector& OutBeamLocation)
{
	// Get current size of viewport
	FVector2D ViewportSize;
	if (GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get screen space location of the crosshairs
	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	// CrosshairLocation.Y -= 50.0f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)	// was deprojection a success?
	{
		FHitResult ScreenTraceHit;
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 50'000.0f };

		OutBeamLocation = End;
		// Set beam endpoint to line trace end point
		GetWorld()->LineTraceSingleByChannel(
			ScreenTraceHit,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		if (ScreenTraceHit.bBlockingHit) // Was there a trace hit?
		{
			// Beam end point is now trace hit location
			OutBeamLocation = ScreenTraceHit.Location;
		}

		// Perform 2nd trace from gun barrel
		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart{ MuzzleSocketLocation };
		const FVector WeaponTraceEnd{ OutBeamLocation };

		GetWorld()->LineTraceSingleByChannel(
			WeaponTraceHit,
			WeaponTraceStart,
			WeaponTraceEnd,
			ECollisionChannel::ECC_Visibility);

		if (WeaponTraceHit.bBlockingHit)	// object between gun barrel and BeamEndPoint
		{
			OutBeamLocation = WeaponTraceHit.Location;
		}
return true;
	}

	return false;
}

void AAnyaCharacter::CharacterAimingTriggered()
{
	bAiming = true;
}

void AAnyaCharacter::CharacterAimingReleased()
{
	bAiming = false;
}

void AAnyaCharacter::CharacterAimZoom(float DeltaTime)
{
	// Set current camera field of view
	if (bAiming)
	{
		// Interpolate to Aiming field of view
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraAimingFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		// Interpolate Default field of view
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	GetThirdPersonCamera()->SetFieldOfView(CameraCurrentFOV);
}

void AAnyaCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AAnyaCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.0f, 600.0f };
	FVector2D VelocityMultiplierRange{ 0.0f, 1.0f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.0f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size());

	// Calc crosshair in air factor
	if (GetCharacterMovement()->IsFalling()) // Is character in air?
	{
		// Spread crosshairs slowly when in air
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	// Character is on ground
	else
	{
		// Shrink crosshairs rapidly when on ground
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			0.0f,
			DeltaTime,
			30.0f);
	}

	// Calc crosshair aiming
	if (bAiming)
	{
		// Shrink crosshairs a small amount quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor, 
			0.6f, 
			DeltaTime, 
			30.0f);
	}
	else
	{
		// Sread crosshairs to Normal quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor, 
			0.0f, 
			DeltaTime, 
			30.0f);
	}

	// True 0.05 seconds after
	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor, 0.3f, 
			DeltaTime, 60.0f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor, 
			0.0f, 
			DeltaTime, 
			60.0f);
	}

	CrosshairSpreadMultiplier = 
		0.5f + 
		CrosshairVelocityFactor + 
		CrosshairInAirFactor -
		CrosshairAimFactor +
		CrosshairShootingFactor;
}

void AAnyaCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;

	GetWorldTimerManager().SetTimer(
		CrosshairShootTimer, 
		this, 
		&AAnyaCharacter::FinishCrosshairBulletFire, 
		ShootTimeDuration);

}

void AAnyaCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void AAnyaCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	StartFireTimer();
}

void AAnyaCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AAnyaCharacter::StartFireTimer()
{
	if (bShouldFire)
	{
		FireWeapon();
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&AAnyaCharacter::AutoFireReset,
			AutomaticFireRate);
	}
}

void AAnyaCharacter::AutoFireReset()
{
	bShouldFire = true;
	if (bFireButtonPressed)
	{
		StartFireTimer();
	}
}

// Called every frame
void AAnyaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle interpolation while aiming
	CharacterAimZoom(DeltaTime);
	// Change look sensitivity based on aiming
	SetLookRates();

	// Calculate crosshair spread multiplier
	CalculateCrosshairSpread(DeltaTime);
}

// Called to bind functionality to input
void AAnyaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	// Player movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AAnyaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAnyaCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAnyaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAnyaCharacter::LookUpAtRate);
	// Mouse inputs
	PlayerInputComponent->BindAxis("Turn", this, &AAnyaCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AAnyaCharacter::LookUp);
	// Player Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	// Player weapon fire & aiming
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAnyaCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AAnyaCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("AimWeapon", IE_Pressed, this, &AAnyaCharacter::CharacterAimingTriggered);
	PlayerInputComponent->BindAction("AimWeapon", IE_Released, this, &AAnyaCharacter::CharacterAimingReleased);
}

float AAnyaCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}