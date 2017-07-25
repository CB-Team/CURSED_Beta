// Fill out your copyright notice in the Description page of Project Settings.

#include "CURSED.h"
#include "Talia.h"


// Sets default values
ATalia::ATalia()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATalia::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATalia::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ATalia::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

