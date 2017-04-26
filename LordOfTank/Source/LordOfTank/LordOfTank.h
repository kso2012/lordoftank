// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"



/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
//#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel1
#define COLLISION_PICKUP		ECC_GameTraceChannel2




#define OP_RECV 1
#define OP_SEND 2
#define MAX_USER 2
#define MAX_ROOM 1
#define SERVER_PORT 4000
#define MAX_BUF_SIZE 4000

#define CS_FORWARD 1
#define CS_RIGHT 2
#define CS_ROOM_CLICK 3

#define SC_ROOM_SHOW 1
#define SC_ROOM_INFO 2
#define SC_MOVE_PLAYER 3

#pragma pack (push, 1)
struct cs_packet_room_click
{
	BYTE size;
	BYTE type;
	int roomNum;
};

struct cs_packet_info
{
	BYTE size;
	BYTE type;
	FString name;
};

struct cs_packet_tank_move
{
	BYTE size;
	BYTE type;
};

struct cs_packet_tank_shot
{
	BYTE size;
	BYTE type;
};

struct cs_packet_dron_move
{
	BYTE size;
	BYTE type;
};

struct cs_packet_dron_targeting
{
	BYTE size;
	BYTE type;
};

struct sc_packet_room_show
{
	BYTE size;
	BYTE type;
	int roomNum;
	int counts;
	int state;
};

struct sc_packet_room_info
{
	BYTE size;
	BYTE type;
	bool canStart;
	bool isReady1;
	bool isReady2;
	int playerNum;
	int counts;
	FString name1;
	FString name2;
};

struct sc_packet_tank_move
{
	BYTE size;
	int num;
	WORD id;
};

#pragma pack (pop)