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
	//�÷��̾� ��ȣ
	int PlayerNum;
	//��ũ�� ������ ALOTPlayer ������
	ALOTPlayer *Tank;
	//����� ������ ALOTDrone ������
	ALOTDrone *Drone;
	//AP�� ������ ����
	float AP;

public:
	//��ũ�� ����� �޾Ƽ� PlayerInfo �� �����Ϳ� �����ϴ� �Լ�
	void InsertPawn(ALOTPlayer *, ALOTDrone *, int);
};

class LORDOFTANK_API PlayerInfoSaved {
public:
	PlayerInfoSaved();

	PlayerInfo PlayerInfoData[2];

	void SavePlayerInfo(int, ALOTPlayer *, ALOTDrone *, float);
};