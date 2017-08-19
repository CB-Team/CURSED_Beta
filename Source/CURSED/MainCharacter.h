// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class CURSED_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

		////Variables

		//TurnRate
		UPROPERTY(EditAnyWhere)
		float TurnRate;

	//DeltaTime template
	UPROPERTY(EditAnyWhere)
		float Delta_Time;

	//Create a SpringArm
	UPROPERTY(EditAnyWhere)
		USpringArmComponent *SpringArmComponent;

	//Create a CameraComponent
	UPROPERTY(EditAnyWhere)
		UCameraComponent *Player_CameraComponent;

	//Vectors & Rotaors
	UPROPERTY(EditAnyWhere)
		FVector2D MovementInput;

	UPROPERTY(EditAnyWhere)
		//MouseAxis.X (Yaw Aixs) && MouseAxis.Y (Pitch Axis)
		FVector2D MouseAxis;

	UPROPERTY(EditAnyWhere)
		FRotator Look_RL;

	UPROPERTY(EditAnyWhere)
		FRotator Look_UpDown;



public:

	// Sets default values for this character's properties
	AMainCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//handle FB Movement
	UFUNCTION()
		void MoveFB(float AxisValue);

	//handle RL Movement
	UFUNCTION()
		void MoveRL(float AxisValue);

	//handle Camera UpDown Looks 
	UFUNCTION()
		void LookUpDown(float AxisValue);

	//handle Camera RL Looks
	UFUNCTION()
		void LookRL(float AxisValue);


	//Set when the character whants to Run (Idle -> Walk -> Run)
	UFUNCTION()
		bool bStartRunning();

	//Set when the character wants to Stop Running ( Run -> Walk -> Idle)
	UFUNCTION()
		bool bStopRunning();

	//Update CharacterMovementSpeed
	UFUNCTION()
		void Update_MaxWalkSpeed();

};