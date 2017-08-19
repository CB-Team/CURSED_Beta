// Fill out your copyright notice in the Description page of Project Settings.

#include "CURSED.h"
#include "MainCharacter.h"

//MainCharacter Constructor
AMainCharacter::AMainCharacter()
{
	///Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	////Set The character SkeletalMesh

	//Get the Refrence of the MainCharacter from the content Data
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MainCharacter_Mesh(TEXT("SkeletalMesh'/Game/Characters/Ganfaul/Ganfaul_M_Aure.Ganfaul_M_Aure'"));

	//Set the Default SkeletalMesh
	if (MainCharacter_Mesh.Succeeded() && GetMesh())
	{
		GetMesh()->SetSkeletalMesh(MainCharacter_Mesh.Object);
	}

	//initialize the location of the Mesh
	GetMesh()->SetRelativeLocation(FVector(0, 0, -101));

	//Fix the Rotation of the character (not always needed it depends!)
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));


	///Set the hieght & the radius of the CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(40, 100);

	///Initialize Variables
	MovementInput.X = MovementInput.Y = 0;
	MouseAxis.X = MouseAxis.Y = 0;
	TurnRate = 45;


	////Set the SpringArmComponent

	//Create a SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	//Attach the SpringArm to the Root
	SpringArmComponent->AttachTo(GetRootComponent());

	//Initialize the Location of the SpringArm
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 75));

	//Set the Arm lenght
	SpringArmComponent->TargetArmLength = 375;

	//Set the Arm rotation to be based on the controller
	SpringArmComponent->bUsePawnControlRotation = true;


	////Set the Player_CameraComponent

	//Create a CameraComponent
	Player_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	//Attach the Camera to the SpringArm

	Player_CameraComponent->AttachTo(SpringArmComponent);

	//Set the Camera rotation to be based on the controller not the SpringArm
	Player_CameraComponent->bUsePawnControlRotation = false;


	///Set The Player on the World
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Spawned;

	///Set UsingController Rotation to false which means the character will not rotate when the controller rotates
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;

	////Set the Character Movement

	//initialize MaxSpeed value
	GetCharacterMovement()->MaxWalkSpeed = 0;

	//Set the Character to be move in the direction of input (input rotation direction!)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	////Set the Animation Mode & AnimClass for the Mesh 

	//Set the Animation Mode that will be used to AnimationBlueprint
	if (GetMesh()) { GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); }

	//Set the AnimClass to be Animation_Bp Class
	ConstructorHelpers::FObjectFinder<UAnimBlueprint>Animation_Bp(TEXT("AnimBlueprint'/Game/AnimationBluePrint/Animation_Bp.Animation_Bp'"));
	if (Animation_Bp.Succeeded() && GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(Animation_Bp.Object->GetAnimBlueprintGeneratedClass());
	}


}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///Store the DeltaTime value (or we can acces the DeltaTime immeditly using GetWorld()->DeltaTime)
	Delta_Time = DeltaTime;



	////Update MaxWalkSpeed Value
	Update_MaxWalkSpeed();

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	////BindAxis to Input

	//Bind FB & RL Movement
	InputComponent->BindAxis("MoveFB", this, &AMainCharacter::MoveFB);
	InputComponent->BindAxis("MoveRL", this, &AMainCharacter::MoveRL);

	//Bind UpDown & RL Looking
	InputComponent->BindAxis("LookUpDown", this, &AMainCharacter::LookUpDown);
	InputComponent->BindAxis("LookRL", this, &AMainCharacter::LookRL);

}

void AMainCharacter::MoveFB(float AxisValue)
{
	//Store the AxisValue to the MovementInput.X (not necessarly!)
	MovementInput.X = AxisValue;

	//check if there is a controller && the FB movement keys are pressed (>0 means Forwad & <0 means backward)
	if (Controller != nullptr && MovementInput.X != 0)
	{
		//Add Movement to the character based on the direction of the controller
		AddMovementInput(Controller->GetActorForwardVector(), MovementInput.X);
	}
}

void AMainCharacter::MoveRL(float AxisValue)
{
	//Store the AxisValue to the MovementInput.Y (not necessarly!)
	MovementInput.Y = AxisValue;

	//check if there is a controller && the FB movement keys are pressed (>0 means Right & <0 means Left)
	if (Controller != nullptr && MovementInput.Y != 0)
	{
		//Add Movement to the character based on the direction of the controller
		AddMovementInput(Controller->GetActorRightVector(), MovementInput.Y);
	}
}

void AMainCharacter::LookUpDown(float AxisValue)
{
	//Store the AxisValue to the MouseAxis.Y (not necessarly!)
	MouseAxis.Y = AxisValue;

	//Add Controller input on pitch Axis every frame
	AddControllerPitchInput(MouseAxis.Y * TurnRate * Delta_Time);
}

void AMainCharacter::LookRL(float AxisValue)
{
	//Store the AxisValue to the MouseAxis.X (not necessarly!)
	MouseAxis.X = AxisValue;

	//Add Controller input on Yaw Axis every frame
	AddControllerYawInput(MouseAxis.X * TurnRate * Delta_Time);
}

bool AMainCharacter::bStartRunning()
{
	//Check if the (Right-Shift || Left-Shift ) is Pressed (still Down) then return true
	if (Controller->CastToPlayerController()->IsInputKeyDown(EKeys::RightShift) || Controller->CastToPlayerController()->IsInputKeyDown(EKeys::LeftShift))
	{
		return true;
	}
	//else means they are not pressed!
	else
		return false;
}

bool AMainCharacter::bStopRunning()
{
	//Check if the (Right-Shift || Left-Shift ) is not Pressed then return true
	if (!Controller->CastToPlayerController()->IsInputKeyDown(EKeys::RightShift) || !Controller->CastToPlayerController()->IsInputKeyDown(EKeys::LeftShift))
	{
		return true;
	}
	//else means they are pressed (still Down!)
	else
		return false;
}

//handle the Update of Idle_Walk_Run by setting the MaxWalkSpeed for each state
void AMainCharacter::Update_MaxWalkSpeed()
{

	//check if the character is moveing if so increase the MaxWalkSpeed depends on its value
	if (!MovementInput.IsZero())
	{
		//if MaxWalkSpeed is <= 200 then the Walk animation is Active
		if (GetCharacterMovement()->MaxWalkSpeed < 200)
		{
			//increase the value of the MaxWalkSpeed by 3 untill it reaches 200 then stay at that speed (Walking)
			do { GetCharacterMovement()->MaxWalkSpeed += 3; } while (GetCharacterMovement()->MaxWalkSpeed == 200);
		}
		//if the (500 >= MaxWalkSpeed >=200 ) the transition from Walking To Running is Active untill it reach 500 (Stay in Running State) 
		else if (GetCharacterMovement()->MaxWalkSpeed >= 200 && GetCharacterMovement()->MaxWalkSpeed <= 500 && bStartRunning())
		{
			if (GetCharacterMovement()->MaxWalkSpeed < 400)
			{
				do { GetCharacterMovement()->MaxWalkSpeed += 3; } while (GetCharacterMovement()->MaxWalkSpeed == 400);
			}
			else
			{
				do { GetCharacterMovement()->MaxWalkSpeed += 1; } while (GetCharacterMovement()->MaxWalkSpeed == 500);
			}
		}
		//When the Shift key is Released the Character will go back smootly to Walking State
		else if (GetCharacterMovement()->MaxWalkSpeed > 200 && bStopRunning())
		{
			do { GetCharacterMovement()->MaxWalkSpeed -= 4; } while (GetCharacterMovement()->MaxWalkSpeed == 200);
		}
		else
		{
			/*.-.*/
		}

	}
	//reset the the WalkSpeed to 0 when there's no movement
	//check if the character was at walking state 
	else if (MovementInput.IsZero() && GetCharacterMovement()->MaxWalkSpeed >0 && GetCharacterMovement()->MaxWalkSpeed <= 200)
	{
		do { GetCharacterMovement()->MaxWalkSpeed -= 4.5; } while (GetCharacterMovement()->MaxWalkSpeed == 0);
	}
	//check if the character at Running State
	else if (MovementInput.IsZero() && GetCharacterMovement()->MaxWalkSpeed >200 && GetCharacterMovement()->MaxWalkSpeed <= 500)
	{
		do { GetCharacterMovement()->MaxWalkSpeed -= 8.5; } while (GetCharacterMovement()->MaxWalkSpeed == 0);
	}
	else
	{
		/*.-.*/
	}

}