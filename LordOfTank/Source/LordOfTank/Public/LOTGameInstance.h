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

#define		RESTOREHP		0
#define		RESTORESHIELD	1

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

USTRUCT(BlueprintType)
struct FSaveItemInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
		int itemNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
		FVector item_Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
		FRotator item_Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
		bool bIsAlive;
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

	UFUNCTION(BlueprintCallable, Category = "Lobby")
		void ReturnMain();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
		void ResetVar();

	UFUNCTION(BlueprintCallable, Category = "Main")
		void Disconnect();

	//CS_FINISH_LOA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "room")
		TArray<FShowRoom> RoomInfo;
		//FShowRoom RoomInfo;

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

	void ProcessPacket(char *ptr);

	void InitEvent(SOCKET sock);

	void connectProc(int index);

	void DeleteSocket(int index);

	void CloseProc(int index);

	void SendLocationInfo(FVector LinearVel, FVector AngularVel, FVector Location, FRotator Rotation, FVector DroneLocation, FRotator DroneRotation);

	void SendFinishLoad();

	void SendFire(FVector Location, FRotator Rotation, float Power,int Type);

	void SendTankHit(float Damage,int Type);

	void SendExplosion();

	void SendTargeting(bool bTarget);

	void SendActivateHoming();

	void SendActivateBeam();

	void SendDeactivateBeam();

	// 비행기 및 아이템 관련 함수
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SendPlaneLocation(FVector P_Location, FRotator P_Rotation);

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SendDeletePlane();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SendSpawnItem();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SendItemLocation(int BoxIndex, FVector I_Location, FRotator I_Rotation, float p_Size);

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SendEatItem(int itemNum);

	void SendAdjustItem(int itemNum);

	
	

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

	//int ProjectileType;

	int EnemyShotProjectileType;

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
	bool bUseableDrone;
	bool bChangeTurnMS;//턴 메세지를 받았는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playing")
		bool bIsWaiting;
	//----


	//게임 종료
	bool bRecvIsEndGame;
	int EndState;

	//타겟팅 상태
	bool bIsTargetMS;
	bool bIsTarget;
	//-----

	//유도 전환상태
	bool bEnemyActivateHoming;
	//

	//emp맞았을 경우
	bool bIsHitEmp;
	bool bRecvHitMS;
	float HitEmpPlayerNum;
	//

	//beam 사용
	bool bRecvBeamMS;
	bool bBeamIsActivated;


	//item 생성 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bIsCreatingItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int planeSpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FVector itemSpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int D_itemNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int E_itemNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int itemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bIsPlaneAlive;
	FSaveItemInfo ItemsInfo[100];

	//plane & item 위치와 회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FVector P_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FRotator P_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FVector I_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FRotator I_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float Parachute_Size;

	//item 정보
	bool bAteHP;
	float delta_HP;
	bool bAteSHIELD;
	float delta_SHIELD;
	bool bAteAP;
	bool bAte_ARMOR_PIERCE;
	bool bAte_HOMING;
	bool bAte_EMP;

};