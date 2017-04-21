// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LOTPlayer.h"
#include "LOTDrone.h"

/**
 * 
 */
class LORDOFTANK_API PlayerInfo
{
public:
	PlayerInfo();
	PlayerInfo(int num, ALOTPlayer *T, ALOTDrone *D, float ActionPoint) {
		PlayerNum = num;
		Tank = T;
		Drone = D;
		AP = ActionPoint;
	}

private:
	//플레이어 번호
	int PlayerNum;
	//탱크를 저장할 ALOTPlayer 포인터
	ALOTPlayer *Tank;
	//드론을 저장할 ALOTDrone 포인터
	ALOTDrone *Drone;
	//AP를 저장할 변수
	float AP;

public:
	//탱크와 드론을 받아서 PlayerInfo 내 포인터에 저장하는 함수
	void InsertPawn(ALOTPlayer *, ALOTDrone *, int);
};

class LORDOFTANK_API PlayerInfoSaved {
public:
	PlayerInfoSaved();

	PlayerInfo PlayerInfoData[2];

	void SavePlayerInfo(int, ALOTPlayer *, ALOTDrone *, float);
};