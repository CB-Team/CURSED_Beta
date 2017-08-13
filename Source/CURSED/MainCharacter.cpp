// Fill out your copyright notice in the Description page of Project Settings.

#include "CURSED.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set The character SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MainCharacter_Mesh(TEXT("SkeletalMesh'/Game/Characters/FemaleWarrior_UE-Scaled_.FemaleWarrior_UE-Scaled_'"));
	GetMesh()->SetSkeletalMesh(MainCharacter_Mesh.Object);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -95));
	GetCapsuleComponent()->SetCapsuleHalfHeight(93);


	///Initialize Variables
	MovementInput.X = MovementInput.Y = 0;
	MouseAxis.X = MouseAxis.Y = 0;

	//Set the PlayerScence
	Player_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerScene"));
	Player_Scene->AttachTo(GetMesh());
	Player_Scene->SetRelativeLocation(FVector(0, 0, 150));

	//Set the SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->AttachTo(Player_Scene);
	SpringArmComponent->SetRelativeRotation(FRotator(-25, 0, 0));
	SpringArmComponent->TargetArmLength = 350;

	//Set the Player_CameraComponent
	Player_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Player_CameraComponent->AttachTo(SpringArmComponent);

	//Set The Player to be Possess to the World
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Spawned;

	//Set UsingController Rotation to false
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;

	//Set Character Movement Properties
	GetCharacterMovement()->bAlwaysCheckFloor = true;
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->MaxWalkSpeed = 200;


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

	Look_UpDown = Player_Scene->GetComponentRotation();
	Look_UpDown.Pitch += MouseAxis.Y;
	Player_Scene->SetWorldRotation(Look_UpDown);

	Look_RL = GetActorRotation();
	Look_RL.Yaw += MouseAxis.X;
	SetActorRotation(Look_RL);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	///BindAxis to Inputs

	//Bind FB & RL Movement
	InputComponent->BindAxis("MoveFB", this, &AMainCharacter::MoveFB);
	InputComponent->BindAxis("MoveRL", this, &AMainCharacter::MoveRL);

	//Bind UpDown & RL Looking
	InputComponent->BindAxis("LookUpDown", this, &AMainCharacter::LookUpDown);
	InputComponent->BindAxis("LookRL", this, &AMainCharacter::LookRL);

}

void AMainCharacter::MoveFB(float AxisValue)
{
	MovementInput.X = AxisValue;
	AddMovementInput(GetActorForwardVector(), MovementInput.X);
}

void AMainCharacter::MoveRL(float AxisValue)
{
	MovementInput.Y = AxisValue;
	AddMovementInput(GetActorRightVector(), MovementInput.Y);
}

void AMainCharacter::LookUpDown(float AxisValue)
{
	MouseAxis.Y = AxisValue;
}

void AMainCharacter::LookRL(float AxisValue)
{
	MouseAxis.X = AxisValue;
}