// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ACOAAvatar::ACOAAvatar() :
	RunSpeed(500.0f),
	Stamina(100.0f),
	MaxStamina(100.0f),
	StaminaDrainRate(20.0f),
	StaminaGainRate(15.0f),
	bRunning(false),
	bStaminaDrained(false)
{
	PrimaryActorTick.bCanEverTick = true;

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->TargetArmLength = 300.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	// controller rotations
	mCamera->bUsePawnControlRotation = false;
	mSpringArm->bUsePawnControlRotation = true;

	// pawn controller rotation yaw
	bUseControllerRotationYaw = false;
}

void ACOAAvatar::Tick(float DeltaTime)
{
	// Will update stamina here
	if (bRunning && !bStaminaDrained)
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Walking)
		{
			Stamina -= StaminaDrainRate * DeltaTime;

			if (Stamina <= 0.0f)
			{
				bRunning = false;
				bStaminaDrained = true;
				UpdateMovementParams();
			}
		}
	}
	else if (!bRunning)
	{
		if (Stamina < MaxStamina)
			Stamina += StaminaGainRate * DeltaTime;
		else
		{
			bStaminaDrained = false;
			Stamina = MaxStamina;
		}
	}
}


// An override function
void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACOAAvatar::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACOAAvatar::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACOAAvatar::RunReleased);
}

void ACOAAvatar::UpdateMovementParams()
{
	if (!bStaminaDrained)
		if (bRunning)
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		else
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	else
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void ACOAAvatar::RunPressed()
{
	if (Stamina > 0.0f && !bStaminaDrained)
	{
		bRunning = true;
		UpdateMovementParams();
	}
}

void ACOAAvatar::RunReleased()
{
	if (bRunning)
	{
		bRunning = false;
		UpdateMovementParams();
	}
}

void ACOAAvatar::MoveForward(float Value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Value);
}

void ACOAAvatar::MoveRight(float Value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, Value);
}
