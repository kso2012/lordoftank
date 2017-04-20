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

	//��ũ�� ������ ALOTPlayer ������
	ALOTPlayer *Tank;
	//����� ������ ALOTDrone ������
	ALOTDrone *Drone;
	//AP�� ������ ����
	float AP;

	//��ũ�� ����� �޾Ƽ� PlayerInfo �� �����Ϳ� �����ϴ� �Լ�
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

	//�÷��̾ ���ϱ� ���� ī����
	UPROPERTY(BlueprintReadWrite)
		int PlayerCount;



};
