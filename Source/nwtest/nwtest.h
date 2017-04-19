// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#define OP_RECV 1
#define OP_SEND 2
#define MAX_USER 2
#define SERVER_PORT 4000
#define MAX_BUF_SIZE 4000

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
