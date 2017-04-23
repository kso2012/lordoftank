// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTDrone.h"
#include "LOTPlayer.h"
#include "PlayerInfo.h"
#include "LordOfTankGameModeBase.h"



PlayerInfo Info = PlayerInfo::PlayerInfo();

ALordOfTankGameModeBase::ALordOfTankGameModeBase()
{
	// set default pawn class to our Blueprinted character 
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SimpleGame/Blueprints/MyCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	DefaultPawnClass = ALOTPlayer::StaticClass();
	PlayerCount = 0;
	PlayerTurn = 0;
	//}
	
}

void ALordOfTankGameModeBase::StartPlay()
{
	Super::StartPlay();

	
}



void ALordOfTankGameModeBase::SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum) {
	Info.InsertPawn(T, D, PlayerNum);
	ALordOfTankGameModeBase::IncreasePlayerCount();
}

void ALordOfTankGameModeBase::IncreasePlayerCount() {
	ALordOfTankGameModeBase::PlayerCount ++;
}


//float ALordOfTankGameModeBase::ReturnProjectileVelocity() {
//	//ALordOfTankGameModeBase *mode = this;
//	ALOTPlayer *Tank = Info.ReturnData(PlayerTurn).ReturnTank();
//	return (*Tank).ReturnVelocity();
//}