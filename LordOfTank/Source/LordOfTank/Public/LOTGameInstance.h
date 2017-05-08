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

UENUM(BlueprintType)
namespace EGameState
{
	enum Type
	{
		
		Mode_Main				UMETA(DisplayName = "MainScreen"),
		
		Mode_SinglePlay			UMETA(DisplayName = "SinglePlay"),
		
		Mode_Multiplay	UMETA(DisplayName = "Multiplay"),
		
		Mode_tutorial			UMETA(DisplayName = "tutorial")
	};
}



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

	FShowRoom()
	{
		roomNum = 0;
		
		counts = 0;
		
		state = 0;
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
	int counts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	FString name1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	FString name2;

	FShowLobby() {
		canStart = false;
		
		isReady1 = false;
		
		isReady2 = false;
		
		counts = 0;
		
		name1 = "none";
		
		name2 = "none";
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
		bool ClickIpEntBT();

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickNameEntBT();

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickReadyBT();

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickBackRoomBT();

	UFUNCTION(BlueprintCallable, Category = "RoomClick")
		bool ClickRoomBT(int roomnum);

	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickStartBT();

	//CS_FINISH_LOA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "room")
		FShowRoom RoomInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "lobby")
		FShowLobby LobbyInfo;
	//TArray<FShowLobby> LobbyInfo
	UPROPERTY()
		FStreamableManager AssetLoader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameState)
		TEnumAsByte<EGameState::Type> GameStateEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "lobby")
		int PlayerNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "keypad")
		FString IPaddr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "keypad")
		FString Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "keypad")
		bool bIsConnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "keypad")
		bool bIsStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
		int LeftTime;

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

	void eventThread();

	static void ToCalleventThread(LPVOID p);

	void ReadPacket(int index);

	void ProcessPacket(char *ptr);

	void InitEvent(SOCKET sock);

	void connectProc(int index);

	void DeleteSocket(int index);

	void CloseProc(int index);

	void SendLocationInfo(FVector LinearVel, FVector AngularVel, FVector Location, FRotator Rotation, FVector DroneLocation, FRotator DroneRotation);

	void SendFinishLoad();

	void SendFire(FVector Location, FRotator Rotation, float Power);

	void SendTankHit(float Damage);

	//적 무브먼트 동기화
	FVector EnemyWorldLocation;
	
	FVector EnemyLinearVelocity;
	
	FVector EnemyAngularVelocity;
	
	FRotator EnemyRotation;

	FVector EnemyDroneLocation;
	
	FRotator EnemyDroneRotation;
	//----

	//포탄 동기화
	FVector EnemyShotLocation;

	float EnemyShotPower;

	FRotator EnemyShotRotation;

	bool bEnemyIsShot;

	//----

	//체력,실드 동기화

	float HP;
	float Shield;
	float HitPlayerNum;
	bool bIsHurt;
	//----



	//턴 변경시 동기화
	float ChargingAP;

	bool bIsmyTurn;

	bool bChangeTurn;
	//----
};