// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTDrone.h"
#include "LOTPlayer.h"
#include "LordOfTankGameModeBase.h"




ALordOfTankGameModeBase::ALordOfTankGameModeBase()
{
	// set default pawn class to our Blueprinted character 
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SimpleGame/Blueprints/MyCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	DefaultPawnClass = ALOTPlayer::StaticClass();
	//}
	
}

void ALordOfTankGameModeBase::StartPlay()
{
	Super::StartPlay();

	
}