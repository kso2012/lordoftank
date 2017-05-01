// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "AllowWindowsPlatformTypes.h"
#include <iostream>
#include "WinSock2.h"
#include <thread>
#include <mutex>
#include <vector>
#include "Tank.h"
#include <set>
#include "HideWindowsPlatformTypes.h"
#include "nwtestGameModeBase.generated.h"

using namespace std;

struct OverlapEx
{
	WSAOVERLAPPED original_overlap;
	int   operation;
	WSABUF recv_buf;
	unsigned char socket_buf[MAX_BUF_SIZE];
	int packet_size;
};

struct Player {
	ATank* Tank;
};

struct Client {
	int   m_id;
	bool is_connected;
	SOCKET m_s;
	Player m_player;
	int roomNum;
	string name;
	bool isReady;
	int playerNum;
	int state; // 클라이언트의 상태 1이면 로비, 2이면 대기방, 3이면 게임시작
	mutex vl_lock;
	OverlapEx m_recv_overlap;
	int previous_data;
	unsigned char packet[MAX_PACKET_SIZE];
};

struct Room {
	int roomNum;
	int counts;
	int state; // 방의 상태
	bool canStart;
	Client* client1;
	Client* client2;
};


/**
*
*/
UCLASS()
class NWTEST_API AnwtestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AnwtestGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void SpawnPlayer(int id);
	void test();

	HANDLE g_hIocp;
	Client clients[MAX_USER];
	Room room;
	bool g_isshutdown;
	bool SpawnActor = false;
	int SpawnID = -1;
	HANDLE hThread;

	//void error_display(char *msg, int err_no);

	void Initialize_Server();

	void workerthread();
	static void ToCallworkerthread(LPVOID p);
	void acceptthread();
	static void ToCallacceptthread(LPVOID p);
	SOCKET new_client;
	SOCKET accept_socket;
	vector <thread *> worker_threads;
	thread* accept_threads;
	bool threadkey;
	float tesking;
	void ProcessPacket(int id, unsigned char *packet);
	void SendPacket(int id, unsigned char *packet);
	int GetID();

};