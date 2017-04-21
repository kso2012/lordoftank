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
//	//탱크를 저장할 ALOTPlayer 포인터
//	ALOTPlayer* Tank;
//	//드론을 저장할 ALOTDrone 포인터
//	ALOTDrone* Drone;
//	//AP를 저장할 변수
//	float AP;
//
//	//탱크와 드론을 받아서 PlayerInfo 내 포인터에 저장하는 함수
//	void InsertPawn(ALOTPlayer *, ALOTDrone *);
//
//};