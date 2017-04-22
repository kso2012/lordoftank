// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	bIsConnected = false;
}

void ULOTGameInstance::ClickEntBT()
{
	
	/*int retval;
	DWORD iobyte;
	cs_packet_room_click *a = reinterpret_cast<cs_packet_room_click *>(send_buffer);
	a->type = CS_ROOM_CLICK;
	a->roomNum = 0;
	a->size = sizeof(cs_packet_room_click);
	send_wsabuf.len = sizeof(cs_packet_room_click);

	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);*/



}

//void ULOTGameInstance::ClickMultiBT()
//{
//	int retval;
//
//	send_wsabuf.buf = send_buffer;
//	send_wsabuf.len = 4000;
//
//	// 윈속 초기화
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		UE_LOG(LogTemp, Warning, TEXT("윈속 초기화 실패"))
//
//		// socket()
//		sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
//	if (sock == INVALID_SOCKET)
//		UE_LOG(LogTemp, Warning, TEXT("소켓 생성 실패"));
//
//	// connect()
//	SOCKADDR_IN serveraddr;
//	ZeroMemory(&serveraddr, sizeof(serveraddr));
//	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.51");
//	serveraddr.sin_port = htons(SERVER_PORT);
//	myevent = WSACreateEvent();
//	WSAEventSelect(sock, myevent, FD_CONNECT);
//
//	retval = WSAConnect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
//	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("위방향 속도= %f"), retval));
//	event_thread = new thread{ &ToCalleventThread, this };
//	bIsConnected = true;
//
//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 실패"));
//
//}

void ULOTGameInstance::SendPos(FVector pos, UWheeledVehicleMovementComponent* VehicleComponent)
{
	int retval;
	Packet *a = reinterpret_cast<Packet *>(send_buffer);

	//a->size = sizeof(Packet);
	//a->VehicleMovement = VehicleComponent;
	send_wsabuf.len = sizeof(Packet);

	DWORD iobyte;

	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval) {
		//int error_code = WSAGetLastError();
		//if (WSA_IO_PENDING != error_code)
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Send Error"));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("센드 성공"));
}

void ULOTGameInstance::ProcessPacket(char *ptr)
{
	switch (ptr[1])
	{
	case SC_MOVE_PLAYER:
		break;
	}
}

void ULOTGameInstance::ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		//printf("Recv Error [%d]\n", err_code);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Recv Error")));
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
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
	while (true)
	{
		WSAWaitForMultipleEvents(2, &myevent, FALSE, WSA_INFINITE, FALSE);
		WSAEnumNetworkEvents(sock, myevent, &ev);
		switch (ev.lNetworkEvents)
		{
		case FD_CONNECT:
			WSAEventSelect(sock, myevent, FD_READ);
			break;
		case FD_READ:
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 받음")));
			ReadPacket(sock);
			break;
		case FD_CLOSE:
			closesocket(sock);
			break;
		}
	}
}

//ULOTGameInstance::~ULOTGameInstance()
//{
//	if (bIsConnected) {
//		closesocket(sock);
//		event_thread->detach();
//		delete(event_thread);
//		WSACleanup();
//	}
//	
//}
