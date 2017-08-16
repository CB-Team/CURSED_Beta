// Fill out your copyright notice in the Description page of Project Settings.

#include "CURSED.h"
#include "CURSEDGameMode.h"

ACURSEDGameMode::ACURSEDGameMode()
{
	//Set the DefualtPawnClass to be the MainCharacter Class
	ConstructorHelpers::FClassFinder<ACharacter>AMainCharacter(TEXT("Class'/Script/CURSED.MainCharacter'"));
	DefaultPawnClass = AMainCharacter.Class;

}


