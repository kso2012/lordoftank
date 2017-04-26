// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{

	bIsTryConnecting = false;
}

void ULOTGameInstance::ClickMultiBT()
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
	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.51");
	serveraddr.sin_port = htons(SERVER_PORT);

	InitEvent(sock);
	WSAEventSelect(app.sarr[app.now], app.hEarr[app.now], FD_CONNECT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	app.now++;
	if (retval != SOCKET_ERROR)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 성공"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 실패"));
	int error_code = WSAGetLastError();
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), error_code));
	// 스레드 생성
	event_thread = new thread{ &ToCalleventThread, this };
	bIsTryConnecting = true;
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("방인원 : %d"), my_packet->counts));
		break;
	}
	case SC_ROOM_INFO:
	{
		sc_packet_room_info *my_packet = reinterpret_cast<sc_packet_room_info*>(ptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("방정보받음!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")));
		LobbyInfo.canStart = my_packet->canStart;
		LobbyInfo.counts = my_packet->counts;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("방인원 : %d"), my_packet->counts));
		LobbyInfo.isReady1 = my_packet->isReady1;
		LobbyInfo.isReady2 = my_packet->isReady2;
		//strncpy(LobbyInfo.name1, my_packet->name1, sizeof(FString));
		//strncpy(LobbyInfo.name2, my_packet->name2, sizeof(FString));
		//FMemory::Memcpy(LobbyInfo.name1, my_packet->name1);
		//FMemory::Memcpy(LobbyInfo.name2, my_packet->name2);
		LobbyInfo.playerNum = my_packet->playerNum;
		break;
	}
	case SC_MOVE_PLAYER:
	{

		break;
	}
	default:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("이상한 데이터 받음")));
		break;
	}
	}
}

void ULOTGameInstance::ReadPacket(int index)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Read Packet 진입")));
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 확실히 받음")));
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
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Read Packet 진입")));
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
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 확실히 받음")));
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
