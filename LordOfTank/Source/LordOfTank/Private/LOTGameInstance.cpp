// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	;
}

void ULOTGameInstance::ClickMultiBT()
{

	WSABUF send_wsabuf;
	char send_buffer[4000];
	int retval;

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = 4000;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		UE_LOG(LogTemp, Warning, TEXT("윈속 초기화 실패"))

		// socket()
		SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		UE_LOG(LogTemp, Warning, TEXT("소켓 생성 실패"));

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("175.196.145.229");
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = WSAConnect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval != SOCKET_ERROR)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 성공"));

	char a = 'A';

	DWORD iobyte;
	memcpy(send_wsabuf.buf, &a, sizeof(a));
	send_wsabuf.len = sizeof(a);

	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval) {
		int error_code = WSAGetLastError();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Send Error"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("센드 성공"));

}