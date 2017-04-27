// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WinSock2.h"
#include <iostream>
#include <string>
#include <thread>
#include "Engine/GameInstance.h"
#include "LOTGameInstance.generated.h"
using namespace std;

//#define SERVER_PORT 4000
#define WM_SOCKET    WM_USER + 1
#define BUF_SIZE 1024

/**
*
*/

struct Packet
{
	BYTE size;
	FVector Pos;
};

struct MyApp
{
	SOCKET sarr[WSA_MAXIMUM_WAIT_EVENTS];
	HANDLE hEarr[WSA_MAXIMUM_WAIT_EVENTS];
	int now;
};


USTRUCT(BlueprintType)
struct FShowRoom
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		int roomNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		int counts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		int state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		bool canStart;

	FShowRoom()
	{
		roomNum = 0;
		counts = 0;
		state = 0;
		canStart = false;
	}

};

USTRUCT(BlueprintType)
struct FShowLobby
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	bool canStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	bool isReady1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	bool isReady2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	int playerNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	int counts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	FString name1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	FString name2;

	FShowLobby() {
		bool canStart = false;
		bool isReady1 = false;
		bool isReady2 = false;
		int playerNum = 0;
		int counts = 0;
		FString name1 = "none";
		FString name2 = "none";
	}

	
};





UCLASS()
class LORDOFTANK_API ULOTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	ULOTGameInstance();

	virtual void FinishDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickMultiBT();

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickReadyBT();

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickBackRoomBT();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "room")
		FShowRoom RoomInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "lobby")
		FShowLobby LobbyInfo;

	UPROPERTY()
		FStreamableManager AssetLoader;

	UFUNCTION(BlueprintCallable, Category = "RoomClick")
		bool ClickRoomBT(int roomnum);


	char send_buffer[4000];
	WSADATA wsa;
	WSABUF   recv_wsabuf;
	char   recv_buffer[BUF_SIZE];
	char   packet_buffer[BUF_SIZE];
	DWORD      in_packet_size = 0;
	int      saved_packet_size = 0;
	SOCKET sock;
	thread* event_thread;
	WSAEVENT myevent;
	HWND main_window_handle = NULL;
	WSABUF send_wsabuf;
	MyApp app;
	WSAOVERLAPPED wsaOverlapped;
	WSAEVENT wsaEvent;
	bool bIsTryConnecting;
	void SendPos(FVector pos);
	void eventThread();
	static void ToCalleventThread(LPVOID p);
	/*void ReadPacket(SOCKET sock);*/
	void ReadPacket(int index);
	void ProcessPacket(char *ptr);
	void InitEvent(SOCKET sock);
	void connectProc(int index);
	void DeleteSocket(int index);
	void CloseProc(int index);

	
};