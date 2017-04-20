// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "LOTPlayer.h"
#include "LOTDrone.h"
#include "LordOfTankGameModeBase.generated.h"

/**
 * 
 */


class PlayerInfo {
public:
	PlayerInfo();

	//탱크를 저장할 ALOTPlayer 포인터
	ALOTPlayer *Tank;
	//드론을 저장할 ALOTDrone 포인터
	ALOTDrone *Drone;
	//AP를 저장할 변수
	float AP;

	//탱크와 드론을 받아서 PlayerInfo 내 포인터에 저장하는 함수
	void InsertPawn(ALOTPlayer *, ALOTDrone *);
};

class FHouse
{
public:
	int32 Size = 10;
};


UCLASS()
class LORDOFTANK_API ALordOfTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALordOfTankGameModeBase();

	class ALOTPlayer* MyPlayer;

	virtual void StartPlay() override; 

	//UPROPERTY(BlueprintReadWrite)
		//PlayerInfo * whichTurn;

	//플레이어를 비교하기 위한 카운터
	UPROPERTY(BlueprintReadWrite)
		int PlayerCount;



};
