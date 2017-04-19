// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WinSock2.h"
#include <iostream>
#include <string>
#include "Engine/GameInstance.h"
#include "LOTGameInstance.generated.h"
using namespace std;

#define SERVER_PORT 4000
#define WM_SOCKET    WM_USER + 1
/**
 * 
 */

UCLASS()
class LORDOFTANK_API ULOTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	ULOTGameInstance();


	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickMultiBT();

	void SendPos(FVector pos);
	HWND main_window_handle = NULL;

	WSABUF send_wsabuf;
	char send_buffer[4000];
	WSADATA wsa;
	SOCKET sock;
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	UPROPERTY()
		FStreamableManager AssetLoader;
};
