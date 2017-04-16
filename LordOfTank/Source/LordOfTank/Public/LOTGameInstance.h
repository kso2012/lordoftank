// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WinSock2.h"
#include <iostream>
#include <string>
#include "Engine/GameInstance.h"
#include "LOTGameInstance.generated.h"
using namespace std;

#define SERVER_PORT 4000

/**
 * 
 */

struct Packet
{
   BYTE size;
   FVector Pos;
};
UCLASS()
class LORDOFTANK_API ULOTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	ULOTGameInstance();


	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickMultiBT();

	void SendPos(FVector pos);

	WSABUF send_wsabuf;
	char send_buffer[4000];
	WSADATA wsa;
	SOCKET sock;
	UPROPERTY()
		FStreamableManager AssetLoader;
};
