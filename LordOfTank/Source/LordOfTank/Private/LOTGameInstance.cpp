// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	;
}

void ULOTGameInstance::ClickMultiBT()
{


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
	retval = WSAConnect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval != SOCKET_ERROR)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ŀ��Ʈ ����"));

	

	

}

void ULOTGameInstance::SendPos(FVector pos)

{

	int retval;
	Packet *a = reinterpret_cast<Packet *>(send_buffer);

	a->size = sizeof(a);
	a->Pos = pos;

	send_wsabuf.len = sizeof(a);

	a->Pos = pos;
	DWORD iobyte;
	//memcpy(send_wsabuf.buf, &a, sizeof(a));
	//send_wsabuf.len = sizeof(a);
	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval) {
		int error_code = WSAGetLastError();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Send Error"));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("���� ����"));
}