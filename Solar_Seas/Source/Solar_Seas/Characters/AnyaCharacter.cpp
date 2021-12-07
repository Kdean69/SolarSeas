// Copyright © 2021 Kdean Games. All Rights Reserved.


#include "AnyaCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AAnyaCharacter::AAnyaCharacter() :
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up camera and boom to character
	// Create Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;	// Follow length
	CameraBoom->bUsePawnControlRotation = true;	// Rotate boom based on controller

	// Create Camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach to CameraBoom
	ThirdPersonCamera->bUsePawnControlRotation = false;	// Camera does not rotate relative to Boom
}

// Called when the game starts or when spawned
void AAnyaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

// Called every frame
void AAnyaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

