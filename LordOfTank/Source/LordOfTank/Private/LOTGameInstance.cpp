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
	serveraddr.sin_addr.s_addr = inet_addr("175.196.145.229");
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = WSAConnect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval != SOCKET_ERROR)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ŀ��Ʈ ����"));

	
	WSAAsyncSelect(sock, main_window_handle, WM_SOCKET, FD_CLOSE | FD_READ);

	

}

void ULOTGameInstance::SendPos(FVector pos)

{

	int retval;
	Packet *a = reinterpret_cast<Packet *>(send_buffer);

	a->size = sizeof(Packet);
	a->Pos = pos;
	send_wsabuf.len = sizeof(Packet);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f"),a->Pos));
	
	DWORD iobyte;
	//memcpy(send_wsabuf.buf, &a, sizeof(a));
	//send_wsabuf.len = sizeof(a);
	retval = WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval) {
		//int error_code = WSAGetLastError();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Send Error"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("X= %f,Y= %f,Z= %f"), a->Pos.X, a->Pos.Y, a->Pos.Z));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("���� ����"));
}

LRESULT CALLBACK ULOTGameInstance::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WindowProc")));
	switch (msg)
	{
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lparam)) {
			closesocket((SOCKET)wparam);
			//clienterror();
			break;
		}
		switch (WSAGETSELECTEVENT(lparam)) {
		case FD_READ:
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("������ ����")));
			break;
		case FD_CLOSE:
			closesocket((SOCKET)wparam);
			//clienterror();
			break;
		}
	}
	default:break;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}