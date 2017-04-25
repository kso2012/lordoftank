// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	bIsConnected = false;
	bIsCreateSocket = false;
	count = 0;
}

void ULOTGameInstance::ClickEntBT()
{
	
	int retval;
	DWORD iobyte;
	cs_packet_room_click *a = reinterpret_cast<cs_packet_room_click *>(send_buffer);
	a->type = CS_ROOM_CLICK;
	a->roomNum = 0;
	a->size = sizeof(cs_packet_room_click);
	send_wsabuf.len = sizeof(cs_packet_room_click);

	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);




}

void ULOTGameInstance::ClickMultiBT()
{
	bIsCreateSocket = true;
	int retval;

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = 4000;

	// ���� �ʱ�ȭ

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		UE_LOG(LogTemp, Warning, TEXT("���� �ʱ�ȭ ����"))

		// socket()
		sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		UE_LOG(LogTemp, Warning, TEXT("���� ���� ����"));

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.51");
	serveraddr.sin_port = htons(SERVER_PORT);
	myevent = WSACreateEvent();
	WSAEventSelect(sock, myevent, FD_CONNECT);

	retval = WSAConnect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("retval �� = %d"), retval));
	
	//if(retval == -1){
	event_thread = new thread{ &ToCalleventThread, this };
	bIsConnected = true;
	//}
	

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ŀ��Ʈ ����"));

}

//void ULOTGameInstance::SendPos(FVector pos, UWheeledVehicleMovementComponent* VehicleComponent)
//{
//	int retval;
//	Packet *a = reinterpret_cast<Packet *>(send_buffer);
//
//	//a->size = sizeof(Packet);
//	//a->VehicleMovement = VehicleComponent;
//	send_wsabuf.len = sizeof(Packet);
//
//	DWORD iobyte;
//
//	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
//	if (retval) {
//		//int error_code = WSAGetLastError();
//		//if (WSA_IO_PENDING != error_code)
//			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Send Error"));
//	}
//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("���� ����"));
//}

void ULOTGameInstance::ProcessPacket(char *ptr)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ProcessPacket ����")));
	switch (ptr[1])
	{
	case SC_ROOM_SHOW:
	{
		sc_packet_room_show *my_packet = reinterpret_cast<sc_packet_room_show*>(ptr);
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("ī��Ʈ = %d ���ȣ = %d"), my_packet->counts, my_packet->roomNum));
		break;
	}
	case SC_MOVE_PLAYER:
		break;
	default:
		break;
		
	}
}

void ULOTGameInstance::ReadPacket(SOCKET sock)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ReadPacket ����")));
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		//printf("Recv Error [%d]\n", err_code);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Recv Error")));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ReadPacket - WSARecv��")));
	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ReadPacket - iobyte != 0")));
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ReadPacket - iobyte üũ ��")));
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
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("FD_CONNECT")));

			WSAEventSelect(sock, myevent, FD_READ);
			break;
		case FD_READ:
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("������ ����")));
			ReadPacket(sock);
			break;
		case FD_CLOSE:
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("FD_CLOSE")));

			closesocket(sock);
			break;
		}
	}
}

void ULOTGameInstance::FinishDestroy()
{
	/*Super::FinishDestroy();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("End")));
	if (bIsCreateSocket)
	{
		if (bIsConnected)
		{
			closesocket(sock);
			event_thread->detach();
			delete(event_thread);
			WSACleanup();
		}
		else
		{
			closesocket(sock);
			WSACleanup();
		}
	}*/
	Super::FinishDestroy();
	count += 1;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("End = %d"),count));

}

//ULOTGameInstance::~ULOTGameInstance()
//{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("������ ����")));
	/*if (bIsConnected) {
			closesocket(sock);
			event_thread->detach();
			delete(event_thread);
			WSACleanup();
		}
		else
		{
			closesocket(sock);
			WSACleanup();
		}
	*/


	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("End")));
	if (bIsCreateSocket)
	{
		if (bIsConnected)
		{
			closesocket(sock);
			event_thread->detach();
			delete(event_thread);
			WSACleanup();
		}
		else
		{
			closesocket(sock);
			WSACleanup();
		}
	}*/

	//count += 1;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("End1 = %d"), count));
//}
