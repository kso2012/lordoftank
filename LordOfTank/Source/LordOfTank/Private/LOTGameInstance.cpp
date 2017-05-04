// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "StartGameMode.h"
#include "GameMode/MultiGameMode.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	bIsConnected = false;
	bIsStart = false;
	bIsTryConnecting = false;
	PlayerNum = 0;
	//Velocity = FVector(0.f,0.f,0.f);
	//Angular = FVector(0.f,0.f,0.f);
}

bool ULOTGameInstance::ClickIpEntBT()
{
	int retval;

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = 4000;

	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		UE_LOG(LogTemp, Warning, TEXT("윈속 초기화 실패"))

	// socket()
		sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		UE_LOG(LogTemp, Warning, TEXT("소켓 생성 실패"));

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//serveraddr.sin_addr.s_addr = inet_addr(TCHAR_TO_UTF8(*IPaddr));
	serveraddr.sin_port = htons(SERVER_PORT);

	InitEvent(sock);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	int error_code = WSAGetLastError();
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), error_code));
	if (retval == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 성공"));
		WSAEventSelect(app.sarr[app.now], app.hEarr[app.now], FD_CONNECT);
		app.now++;
		// 스레드 생성
		event_thread = new thread{ &ToCalleventThread, this };
		bIsTryConnecting = true;
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 실패"));
		bIsTryConnecting = false;
		bIsConnected = false;
		closesocket(sock);
		WSACleanup();
		return false;
	}

	

}

void ULOTGameInstance::SendPos(FVector pos)
{
	/*int retval;
	Packet *a = reinterpret_cast<Packet *>(send_buffer);

	a->size = sizeof(Packet);
	a->Pos = pos;
	send_wsabuf.len = sizeof(Packet);

	DWORD iobyte;

	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval) {
	int error_code = WSAGetLastError();
	if(WSA_IO_PENDING != error_code)
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Send Error"));
	}*/
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("센드 성공"));
}

void ULOTGameInstance::ClickNameEntBT()
{
	cs_packet_player_name *player_name = reinterpret_cast<cs_packet_player_name *>(send_buffer);
	player_name->name = TCHAR_TO_UTF8(*Nickname);
	player_name->type = CS_PLAYER_NAME;
	player_name->size = sizeof(cs_packet_player_name);
	send_wsabuf.len = sizeof(cs_packet_player_name);
	DWORD iobyte;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Nickname);
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);


}

void ULOTGameInstance::ProcessPacket(char *ptr)
{
	switch (ptr[1])
	{
	case SC_ROOM_SHOW:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("방정보받음")));
		sc_packet_room_show *my_packet = reinterpret_cast<sc_packet_room_show*>(ptr);
		RoomInfo.roomNum = my_packet->roomNum;
		RoomInfo.counts = my_packet->counts;
		RoomInfo.state = my_packet->state;
		break;
	}
	case SC_PLAYER_NUM:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("플레이어넘버받음")));
		sc_packet_player_num *my_packet = reinterpret_cast<sc_packet_player_num*>(ptr);
		PlayerNum = my_packet->playerNum;
		break;
	}
	case SC_ROOM_INFO:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("로비정보받음")));
		sc_packet_room_info *my_packet = reinterpret_cast<sc_packet_room_info*>(ptr);
		LobbyInfo.canStart = my_packet->canStart;
		LobbyInfo.counts = my_packet->counts;
		LobbyInfo.isReady1 = my_packet->isReady1;
		LobbyInfo.isReady2 = my_packet->isReady2;
		LobbyInfo.name1 = my_packet->name1.c_str();
	    LobbyInfo.name2 = my_packet->name2.c_str();
		break;
	}

	case SC_ROOM_READY:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("레디정보받음")));
		sc_packet_room_ready *my_packet = reinterpret_cast<sc_packet_room_ready*>(ptr);
		LobbyInfo.canStart = my_packet->canStart;
		LobbyInfo.isReady1 = my_packet->isReady1;
		LobbyInfo.isReady2 = my_packet->isReady2;
		break;
	}

	case SC_GAME_START:
	{
		bIsStart = true;
		break;
	}
	case SC_TANK_MOVE:
	{
		
		sc_packet_tank_move *my_packet = reinterpret_cast<sc_packet_tank_move*>(ptr);
		
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f,%f,%f"), my_packet->velocity.X, my_packet->velocity.Y, my_packet->velocity.Z));
		
		Location = my_packet->loctaion;
		Velocity = my_packet->velocity;

		
		break;
	}

	default:
	{
		auto a =ptr[1];
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("이상한 데이터 받음")));
		break;
	}
	}
}

void ULOTGameInstance::ReadPacket(int index)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Read Packet 진입")));
	SOCKET sock = app.sarr[index];
	DWORD iobyte, ioflag = 0;
	wsaEvent = WSACreateEvent();
	ZeroMemory(&wsaOverlapped, sizeof(wsaOverlapped));
	wsaOverlapped.hEvent = wsaEvent;
	recv_wsabuf.len = sizeof(recv_buffer);
	recv_wsabuf.buf = recv_buffer;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, &wsaOverlapped, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Recv Error")));
	}
	WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 확실히 받음")));
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}

void ULOTGameInstance::ToCalleventThread(LPVOID p)
{
	((ULOTGameInstance*)p)->eventThread();
}

void ULOTGameInstance::eventThread()
{
	WSANETWORKEVENTS ev;
	int index;
	while (true)
	{
		index = WSAWaitForMultipleEvents(app.now, app.hEarr, FALSE, WSA_INFINITE, FALSE);
		index -= WSA_WAIT_EVENT_0;
		WSAEnumNetworkEvents(app.sarr[index], app.hEarr[index], &ev);
		switch (ev.lNetworkEvents)
		{
		case FD_CONNECT:
			connectProc(index);
			break;
		case FD_READ:
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 받음")));
			//ReadPacket(index);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Read Packet 진입")));
			SOCKET sock = app.sarr[index];
			DWORD iobyte, ioflag = 0;
			wsaEvent = WSACreateEvent();
			ZeroMemory(&wsaOverlapped, sizeof(wsaOverlapped));
			wsaOverlapped.hEvent = wsaEvent;
			recv_wsabuf.len = 1024;
			recv_wsabuf.buf = recv_buffer;

			int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, &wsaOverlapped, NULL);
			if (ret) {
				int err_code = WSAGetLastError();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Recv Error")));
			}
			WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);

			BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

			while (0 != iobyte) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 확실히 받음")));
				if (0 == in_packet_size) in_packet_size = ptr[0];
				if (iobyte + saved_packet_size >= in_packet_size) {
					memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
					ProcessPacket(packet_buffer);
					ptr += in_packet_size - saved_packet_size;
					iobyte -= in_packet_size - saved_packet_size;
					in_packet_size = 0;
					saved_packet_size = 0;
				}
				else {
					memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
					saved_packet_size += iobyte;
					iobyte = 0;
				}
			}
			break;
		}
		case FD_CLOSE:
		{
			CloseProc(index);
			break;
		}
		}
	}
	WSACleanup();
}

void ULOTGameInstance::InitEvent(SOCKET sock)
{
	app.sarr[app.now] = sock;
	app.hEarr[app.now] = WSACreateEvent();
}

void ULOTGameInstance::connectProc(int index)
{
	SOCKET sock = app.sarr[index];
	DeleteSocket(index);

	InitEvent(sock);

	WSAEventSelect(app.sarr[app.now], app.hEarr[app.now], FD_READ);
	app.now++;

}

void ULOTGameInstance::DeleteSocket(int index)
{
	app.now--;
	app.hEarr[index] = app.hEarr[app.now];
	app.sarr[index] = app.sarr[app.now];
}

void ULOTGameInstance::CloseProc(int index)
{
	WSACloseEvent(app.hEarr[index]);
	closesocket(app.sarr[index]);
	DeleteSocket(index);
}


bool ULOTGameInstance::ClickRoomBT(int roomnum)
{
	if (RoomInfo.counts < 2)
	{

		cs_packet_room_click *room_click = reinterpret_cast<cs_packet_room_click *>(send_buffer);
		room_click->type = CS_ROOM_CLICK;
		room_click->size = sizeof(cs_packet_room_click);
		room_click->roomNum = roomnum;
		send_wsabuf.len = sizeof(cs_packet_room_click);

		DWORD iobyte;
		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
		return true;
		
	}
	return false;
}

void ULOTGameInstance::ClickReadyBT()
{
	cs_packet_room_ready *room_ready = reinterpret_cast<cs_packet_room_ready *>(send_buffer);
	room_ready->type = CS_READY_CLICK;
	room_ready->size = sizeof(cs_packet_room_ready);
	send_wsabuf.len = sizeof(cs_packet_room_ready);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}


void ULOTGameInstance::ClickBackRoomBT()
{
	cs_packet_room_exit *room_exit = reinterpret_cast<cs_packet_room_exit *>(send_buffer);
	room_exit->type = CS_EXIT_CLICK;
	room_exit->size = sizeof(cs_packet_room_exit);
	send_wsabuf.len = sizeof(cs_packet_room_exit);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::ClickStartBT()
{
	//static void	Delay(UObject* WorldContextObject, float Duration, struct FLatentActionInfo LatentInfo);
	cs_packet_game_start *game_start = reinterpret_cast<cs_packet_game_start *>(send_buffer);
	game_start->type = CS_GAME_START;
	game_start->size = sizeof(cs_packet_game_start);
	send_wsabuf.len = sizeof(cs_packet_game_start);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}



void ULOTGameInstance::FinishDestroy()
{

	Super::FinishDestroy();

	if (bIsTryConnecting) {
		closesocket(sock);
		event_thread->detach();
		delete(event_thread);
		WSACleanup();
	}

}

void ULOTGameInstance::testfunc()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("게임 스타트")));
	AStartGameMode* const Testa = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Testa->StartMultiGame();
}

void ULOTGameInstance::MoveForward(float val)
{
	cs_packet_tank_move *tank_move = reinterpret_cast<cs_packet_tank_move *>(send_buffer);
	
	tank_move->val = val;
	tank_move->type = CS_TANK_FORWARD;
	tank_move->size = sizeof(cs_packet_tank_move);
	send_wsabuf.len = sizeof(cs_packet_tank_move);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::MoveRight(float val)
{
	cs_packet_tank_move *tank_move = reinterpret_cast<cs_packet_tank_move *>(send_buffer);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("오른쪽 send%f"), val));
	tank_move->val = val;
	tank_move->type = CS_TANK_RIGHT;
	tank_move->size = sizeof(cs_packet_tank_move);
	send_wsabuf.len = sizeof(cs_packet_tank_move);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

