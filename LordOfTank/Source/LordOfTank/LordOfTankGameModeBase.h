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
	//ALordOfTankGameModeBase();

	//class ALOTPlayer* MyPlayer;

	//virtual void StartPlay() override; 

	////플레이어를 비교하기 위한 카운터
	//UPROPERTY(BlueprintReadWrite)
	//	int PlayerCount;

	////플레이어 턴
	//UPROPERTY(BlueprintReadWrite)
	//	int PlayerTurn;

	////플레이어 데이터를 저장하는 함수
	//UFUNCTION(BlueprintCallable)
	//	void SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum);

	////플레이어가 데이터에 저장되면 플레이어 카운트를 증가시킴
	//UFUNCTION(BlueprintCallable)
	//	void IncreasePlayerCount();

	////발사된 투사체의 속도를 반환하는 함수
	//float ReturnProjectileVelocity();

};
