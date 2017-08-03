// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>
#include "Engine.h"

/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
//#define COLLISION_WEAPON      ECC_GameTraceChannel1
#define COLLISION_PROJECTILE   ECC_GameTraceChannel1
#define COLLISION_PICKUP      ECC_GameTraceChannel2

#define OP_RECV 1
#define OP_SEND 2
#define MAX_USER 10
#define MAX_ROOM 5
#define SERVER_PORT 4000
#define MAX_BUF_SIZE 4000

#define CS_PLAYER_NAME 1
#define CS_ROOM_CLICK 2
#define CS_READY_CLICK 3
#define CS_EXIT_CLICK 4
#define CS_GAME_START 5
#define CS_PLAYER_MOVE 6
#define CS_FINISH_LOAD 7
#define CS_TANK_SHOT 8
#define CS_TANK_HIT 9
#define CS_TANK_EXPLOSION 10
#define CS_RETURN_MAIN 11
#define CS_DRONE_TARGETING 12
#define CS_ACTIVATE_HOMING 13
#define CS_BEAM_ON 14
#define CS_BEAM_OFF 15
#define CS_PLANE_MOVE 16
#define CS_SPAWN_ITEM 17
#define CS_ITEM_MOVE 18
#define CS_DELETE_PLANE 19
#define CS_EAT_ITEM 20
#define CS_RESTORE_HP 21
#define CS_RESTORE_SHIELD 22

#define SC_ROOM_SHOW 1
#define SC_PLAYER_NUM 2
#define SC_ROOM_INFO 3
#define SC_ROOM_READY 4
#define SC_GAME_START 5
#define SC_PLAYER_MOVE 6
#define SC_TURN 7
#define SC_TIMER 8
#define SC_TANK_SHOT 9
#define SC_TANK_HIT 10
#define SC_FINISH_GAME 11
#define SC_DRONE_TARGETING 12
#define SC_ACTIVATE_HOMING 13
#define SC_EMP_ON 14
#define SC_EMP_OFF 15
#define SC_BEAM_ON 16
#define SC_BEAM_OFF 17
#define SC_SPAWN_PLANE 18
#define SC_PLANE_MOVE 19
#define SC_SPAWN_ITEM 20
#define SC_ITEM_MOVE 21
#define SC_DELETE_PLANE 22
#define SC_EAT_ITEM 23
#define SC_RESTORE_HP 24
#define SC_RESTORE_SHIELD 25

#define PROJECTILE_COMMON 1
#define PROJECTILE_ARMORPIERCING 2
#define PROJECTILE_HOMING 3
#define PROJECTILE_EMP 4

#define GAME_LOSE 1
#define GAME_WIN 2
#define ENEMY_OUT 3

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

struct cs_packet_finish_load
{
	BYTE size;
	BYTE type;
};

struct cs_packet_player_move
{
	BYTE size;
	BYTE type;
	FVector worldLocation;
	FVector linearVelocity;
	FVector angularVelocity;
	FRotator rotation;
	FVector droneLocation;
	FRotator droneRotation;
};

struct cs_packet_tank_shot
{
	BYTE size;
	BYTE type;
	FVector location;
	float power;
	FRotator rotation;
	int projectile;
};

struct cs_packet_activate_homing
{
	BYTE size;
	BYTE type;
};

struct cs_packet_tank_hit
{
	BYTE size;
	BYTE type;
	float damage;
	int projectile;
};

struct cs_packet_tank_explosion
{
	BYTE size;
	BYTE type;
};

struct cs_packet_drone_targeting
{
	BYTE size;
	BYTE type;
	bool isTargeting;
};

struct cs_packet_return_main
{
	BYTE size;
	BYTE type;
};

struct cs_packet_use_beam
{
	BYTE size;
	BYTE type;
};

struct cs_packet_plane_move
{
	BYTE size;
	BYTE type;
	FVector location;
	FRotator rotator;
};

struct cs_packet_spawn_item
{
	BYTE size;
	BYTE type;
};

struct cs_packet_item_move
{
	BYTE size;
	BYTE type;
	FVector location;
	FRotator rotator;
	float parachuteSize;
	int index;
};

struct cs_packet_delete_plane
{
	BYTE size;
	BYTE type;
};

struct cs_packet_eat_item
{
	BYTE size;
	BYTE type;
	int itemNum;
};

struct cs_packet_adjust_item
{
	BYTE size;
	BYTE type;
};

struct sc_packet_room_show
{
	BYTE size;
	BYTE type;
	int roomNum[MAX_ROOM];
	int counts[MAX_ROOM];
	int state[MAX_ROOM];
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
	bool turn;
};

struct sc_packet_turn
{
	BYTE size;
	BYTE type;
	bool turn;
	float ap;
};

struct sc_packet_timer
{
	BYTE size;
	BYTE type;
	int timer;
	bool isWaiting;
};

struct sc_packet_player_move
{
	BYTE size;
	BYTE type;
	FVector worldLocation;
	FVector linearVelocity;
	FVector angularVelocity;
	FRotator rotation;
	FVector droneLocation;
	FRotator droneRotation;
};

struct sc_packet_tank_shot
{
	BYTE size;
	BYTE type;
	FVector location;
	float power;
	FRotator rotation;
	int projectile;
};

struct sc_packet_activate_homing
{
	BYTE size;
	BYTE type;
};

struct sc_packet_tank_hit
{
	BYTE size;
	BYTE type;
	float hp;
	float shield;
	int playerNum;
	int projectile;
};

struct sc_packet_finish_game
{
	BYTE size;
	BYTE type;
	int state;
};

struct sc_packet_drone_targeting
{
	BYTE size;
	BYTE type;
	bool isTargeting;
};

struct sc_packet_emp
{
	BYTE size;
	BYTE type;
	int playerNum;
};

struct sc_packet_use_beam
{
	BYTE size;
	BYTE type;
};

struct sc_packet_spawn_plane
{
	BYTE size;
	BYTE type;
	int planeLocation;
	FVector itemLocation;
	int itemNum;
};

struct sc_packet_plane_move
{
	BYTE size;
	BYTE type;
	FVector location;
	FRotator rotator;
};

struct sc_packet_spawn_item
{
	BYTE size;
	BYTE type;
};

struct sc_packet_item_move
{
	BYTE size;
	BYTE type;
	FVector location;
	FRotator rotator;
	float parachuteSize;
	int index;
};

struct sc_packet_delete_plane
{
	BYTE size;
	BYTE type;
};

struct sc_packet_eat_item
{
	BYTE size;
	BYTE type;
	int itemNum;
};

struct sc_packet_adjust_item
{
	BYTE size;
	BYTE type;
	int num;
};

#pragma pack (pop)