// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WinSock2.h"
#include <iostream>
#include <string>
#include <thread>
#include "Engine/GameInstance.h"
#include "LOTGameInstance.generated.h"
using namespace std;

#define WM_SOCKET    WM_USER + 1
#define BUF_SIZE 1024
/**
 * 
 */

UCLASS()
class LORDOFTANK_API ULOTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	ULOTGameInstance();
	//~ULOTGameInstance();
	virtual void FinishDestroy() override;
	UFUNCTION(BlueprintCallable, Category = "MultiBTClick")
		void ClickMultiBT();

	UFUNCTION(BlueprintCallable, Category = "EntBTClick")
		void ClickEntBT();
	int count;
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
	bool bIsConnected;
	bool bIsCreateSocket;
	//void SendPos(FVector pos, UWheeledVehicleMovementComponent* VehicleComponent);
	void eventThread();
	static void ToCalleventThread(LPVOID p);
	void ReadPacket(SOCKET sock);
	void ProcessPacket(char *ptr);
	UPROPERTY()
		FStreamableManager AssetLoader;
private:
	

};
