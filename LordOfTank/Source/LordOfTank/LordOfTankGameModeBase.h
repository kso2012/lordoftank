// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "LOTPlayer.h"
#include "LOTDrone.h"
#include "PlayerInfo.h"
#include "LordOfTankGameModeBase.generated.h"

/**
 * 
 */



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

	//플레이어를 비교하기 위한 카운터
	UPROPERTY(BlueprintReadWrite)
		int PlayerCount;

	UFUNCTION(BlueprintCallable)
		void SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum);


	UFUNCTION(BlueprintCallable)
		void IncreasePlayerCount();

};
