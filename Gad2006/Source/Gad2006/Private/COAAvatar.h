// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "COAAvatar.generated.h"


UCLASS()
class ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()

public:
	ACOAAvatar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UCameraComponent* mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	USpringArmComponent* mSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float RunSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Character")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float StaminaGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	bool bRunning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	bool bStaminaDrained;


	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RunPressed();
	void RunReleased();
	void UpdateMovementParams();
};
