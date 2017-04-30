// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include <string>

#define OP_RECV 1
#define OP_SEND 2
#define MAX_USER 2
#define MAX_ROOM 1
#define SERVER_PORT 4000
#define MAX_BUF_SIZE 4000

#define CS_PLAYER_NAME 1
#define CS_ROOM_CLICK 2
#define CS_READY_CLICK 3
#define CS_EXIT_CLICK 4
#define CS_GAME_START 5
#define CS_TANK_FORWARD 6
#define CS_TANK_RIGHT 7

#define SC_ROOM_SHOW 1
#define SC_PLAYER_NUM 2
#define SC_ROOM_INFO 3
#define SC_ROOM_READY 4
#define SC_GAME_START 5
#define SC_MOVE_PLAYER 6

#pragma pack (push, 1)
struct cs_packet_player_name
{
	BYTE size;
	BYTE type;
	std::string name;
};

struct cs_packet_room_click
{
	BYTE size;
	BYTE type;
	int roomNum;
};

struct cs_packet_room_ready
{
	BYTE size;
	BYTE type;
};

struct cs_packet_room_exit
{
	BYTE size;
	BYTE type;
};

struct cs_packet_game_start
{
	BYTE size;
	BYTE type;
};

struct cs_packet_info
{
	BYTE size;
	BYTE type;
	std::string name;
};

struct cs_packet_tank_move
{
	BYTE size;
	BYTE type;
	float val;
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

struct sc_packet_player_num
{
	BYTE size;
	BYTE type;
	int playerNum;
};

struct sc_packet_room_info
{
	BYTE size;
	BYTE type;
	bool canStart;
	bool isReady1;
	bool isReady2;
	int counts;
	std::string name1;
	std::string name2;
};

struct sc_packet_room_ready
{
	BYTE size;
	BYTE type;
	bool canStart;
	bool isReady1;
	bool isReady2;
};

struct sc_packet_game_start
{
	BYTE size;
	BYTE type;
};

struct sc_packet_tank_move
{
	BYTE size;
	BYTE type;
	FVector velocity;
	FVector angle;
};
#pragma pack (pop)