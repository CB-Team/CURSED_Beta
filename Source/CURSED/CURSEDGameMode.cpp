// Fill out your copyright notice in the Description page of Project Settings.

#include "CURSED.h"
#include "CURSEDGameMode.h"

ACURSEDGameMode::ACURSEDGameMode(const FObjectInitializer &ObjectInitializer)
{
	//Set the default Pawn
	ConstructorHelpers::FClassFinder<ACharacter>MainCharacter(TEXT("Class'/Script/CURSED.MainCharacter'"));
	DefaultPawnClass = MainCharacter.Class;

}


