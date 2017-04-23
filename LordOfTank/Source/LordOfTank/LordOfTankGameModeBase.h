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

	////�÷��̾ ���ϱ� ���� ī����
	//UPROPERTY(BlueprintReadWrite)
	//	int PlayerCount;

	////�÷��̾� ��
	//UPROPERTY(BlueprintReadWrite)
	//	int PlayerTurn;

	////�÷��̾� �����͸� �����ϴ� �Լ�
	//UFUNCTION(BlueprintCallable)
	//	void SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum);

	////�÷��̾ �����Ϳ� ����Ǹ� �÷��̾� ī��Ʈ�� ������Ŵ
	//UFUNCTION(BlueprintCallable)
	//	void IncreasePlayerCount();

	////�߻�� ����ü�� �ӵ��� ��ȯ�ϴ� �Լ�
	//float ReturnProjectileVelocity();

};
