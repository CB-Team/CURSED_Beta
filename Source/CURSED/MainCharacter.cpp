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
	//Set it as the Default SkeletalMesh
	GetMesh()->SetSkeletalMesh(MainCharacter_Mesh.Object);
	//initialize the location of the Mesh
	GetMesh()->SetRelativeLocation(FVector(0, 0, -95));
	//Fix the Rotation of the character (not always needed it depends!)
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));


	///Set the hieght of the CapsuleComponent
	GetCapsuleComponent()->SetCapsuleHalfHeight(93);

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
	//Initialize the Camera Rotation

	//Player_CameraComponent->SetRelativeRotation(FRotator(-25, 0, 0));

	//Set the Camera rotation to be based on the controller not the SpringArm
	Player_CameraComponent->bUsePawnControlRotation = false;


	///Set The Player on the World
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Spawned;

	///Set UsingController Rotation to false which means the character will not rotate when the controller rotates
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;

	////Set the Character Movement
	//Set the MaxSpeed value
	GetCharacterMovement()->MaxWalkSpeed = 200;
	//Set the Character to be move in the direction of input (input rotation direction!)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	////Set Some Animations
	//Idle-Animation
	ConstructorHelpers::FObjectFinder<UAnimationAsset>Idle_Animation(TEXT("AnimSequence'/Game/Characters/Ganfaul/Animation/neutral_idle.neutral_idle'"));
	Idle_Anim = Idle_Animation.Object;
	//Walking-Animation
	ConstructorHelpers::FObjectFinder<UAnimationAsset>Walking_Animation(TEXT(""));


}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (EAnimationMode::AnimationSingleNode)
	{
		GetMesh()->PlayAnimation(Idle_Anim, true);
	}

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///Store the DeltaTime value (or we can acces the DeltaTime immeditly using GetWorld()->DeltaTime)
	Delta_Time = DeltaTime;

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	////BindAxis to Inputs
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