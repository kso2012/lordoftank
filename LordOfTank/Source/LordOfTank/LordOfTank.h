// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"



/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
//#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel1
#define COLLISION_PICKUP		ECC_GameTraceChannel2


struct Packet
{
	BYTE size;
	FVector Pos;
};

//struct PlayerInfoa {
//
//
//	//��ũ�� ������ ALOTPlayer ������
//	ALOTPlayer* Tank;
//	//����� ������ ALOTDrone ������
//	ALOTDrone* Drone;
//	//AP�� ������ ����
//	float AP;
//
//	//��ũ�� ����� �޾Ƽ� PlayerInfo �� �����Ϳ� �����ϴ� �Լ�
//	void InsertPawn(ALOTPlayer *, ALOTDrone *);
//
//};