// Fill out your copyright notice in the Description page of Project Settings.

#include "nwtest.h"
#include "nwtestGameModeBase.h"


using namespace std;



AnwtestGameModeBase::AnwtestGameModeBase()
{
	g_isshutdown = false;
	tesking = 0;
	PrimaryActorTick.bCanEverTick = true;
	threadkey = false;

}

void AnwtestGameModeBase::BeginPlay()
{
	Super::BeginPlay();


	//-

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	Initialize_Server();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("beforetocallworker"));
	for (auto i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
		worker_threads.push_back(new thread{ &ToCallworkerthread, this });
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("beforetocallaccept"));


	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("소켓생성전"));
	accept_threads = new thread{ &ToCallacceptthread, this };
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("소켓 생성 성공"));





}

void AnwtestGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("끝")));

	threadkey = true;
	closesocket(new_client);
	closesocket(accept_socket);
	for (auto th : worker_threads)
	{
		th->detach();
		delete th;
	}
	accept_threads->detach();
	delete accept_threads;

	WSACleanup();
}

void AnwtestGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("tesking= %f"), tesking));

}

//void AnwtestGameModeBase::error_display(char *msg, int err_no)
//{
//   WCHAR *lpMsgBuf;
//   FormatMessage(
//      FORMAT_MESSAGE_ALLOCATE_BUFFER |
//      FORMAT_MESSAGE_FROM_SYSTEM,
//      NULL, err_no,
//      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//      (LPTSTR)&lpMsgBuf, 0, NULL);
//   printf("%s", msg);
//   //wprintf(L"에러%s\n", lpMsgBuf);
//   LocalFree(lpMsgBuf);
//}

void AnwtestGameModeBase::Initialize_Server()
{
	WSADATA   wsadata;
	//_wsetlocale(LC_ALL, L"korean");
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	for (auto i = 0; i < MAX_USER; ++i) {
		clients[i].m_recv_overlap.recv_buf.buf =
			reinterpret_cast<CHAR *>(clients[i].m_recv_overlap.socket_buf);
		clients[i].m_recv_overlap.recv_buf.len = MAX_BUF_SIZE;
		clients[i].m_recv_overlap.operation = OP_RECV;
		clients[i].is_connected = false;
	}
}

void AnwtestGameModeBase::ProcessPacket(int id, unsigned char *packet)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ProcessPacket 진입")));

	/*unsigned char type = packet[1];
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%d"), type));*/

	FVector *pos = reinterpret_cast<FVector*>(packet);
	float x, y, z;
	x = pos->X;
	y = pos->Y;
	z = pos->Z;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("X = %f, Y = %f, Z = %f"), x, y, z));

	


}

void AnwtestGameModeBase::workerthread()
{
	DWORD io_size, key;
	OverlapEx *overlap;
	BOOL result;

	while (threadkey == false)
	{
		result = GetQueuedCompletionStatus(g_hIocp, &io_size, reinterpret_cast<PULONG_PTR>(&key),
			reinterpret_cast<LPOVERLAPPED *>(&overlap), INFINITE);

		if (false == result) {
			// Error를 처리한다.
		}

		if (io_size == 0)
		{
			//closesocket(clients[key].m_s);
		}

		if (overlap->operation == OP_RECV)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("데이터 받음")));
			unsigned char *buf_ptr = overlap->socket_buf;
			int remained = io_size;

			while (0 < remained) { // 패킷이 다올때까지 처리해야함.
				if (0 == clients[key].m_recv_overlap.packet_size) {
					clients[key].m_recv_overlap.packet_size = buf_ptr[0];
				}
				int required = clients[key].m_recv_overlap.packet_size
					- clients[key].previous_data;
				if (remained >= required) {
					memcpy(clients[key].packet + clients[key].previous_data,
						buf_ptr,
						required);
					ProcessPacket(key, clients[key].packet);
					remained -= required;
					buf_ptr += required;
					clients[key].m_recv_overlap.packet_size = 0;
					clients[key].previous_data = 0;
				}
				else {
					memcpy(clients[key].packet + clients[key].previous_data,
						buf_ptr, remained);
					clients[key].previous_data += remained;
					remained = 0;
					buf_ptr += remained;
				}
			}

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%c %d"), buf_ptr, io_size));


			DWORD flags = 0;
			WSARecv(clients[key].m_s,
				&clients[key].m_recv_overlap.recv_buf,
				1, NULL, &flags,
				reinterpret_cast<LPWSAOVERLAPPED>(&clients[key].m_recv_overlap),
				NULL);
		}
		else if (overlap->operation == OP_SEND)
		{

		}
		else
		{
			cout << "Unknown Event on worker_thread" << endl;
			while (true);
		}
	}
}

void AnwtestGameModeBase::acceptthread()
{

	struct sockaddr_in listen_addr;

	accept_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(&listen_addr, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(SERVER_PORT);
	ZeroMemory(&listen_addr.sin_zero, 8);
	::bind(accept_socket,
		reinterpret_cast<sockaddr *>(&listen_addr),
		sizeof(listen_addr));

	listen(accept_socket, 10);

	while (threadkey == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Accept"));
		struct sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);

		new_client = WSAAccept(accept_socket,
			reinterpret_cast<sockaddr *>(&client_addr),
			&addr_size, NULL, NULL);


		int new_id = -1;
		for (auto i = 0; i < MAX_USER; ++i) {
			if (clients[i].is_connected == false) {
				new_id = i;
				break;
			}
		}

		if (-1 == new_id) {
			closesocket(new_client);
			tesking += 1;
			continue;
		}

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client),
			g_hIocp, new_id, 0);

		clients[new_id].m_s = new_client;
		clients[new_id].is_connected = true;
		clients[new_id].m_id = new_id;
		clients[new_id].m_recv_overlap.operation = OP_RECV;
		clients[new_id].m_recv_overlap.packet_size = 0;
		clients[new_id].previous_data = 0;

		DWORD flags = 0;
		int res = WSARecv(new_client,
			&clients[new_id].m_recv_overlap.recv_buf, 1, NULL,
			&flags,
			&clients[new_id].m_recv_overlap.original_overlap, NULL);
	}
}


void AnwtestGameModeBase::ToCallworkerthread(LPVOID p)
{
	((AnwtestGameModeBase*)p)->workerthread();
}

void AnwtestGameModeBase::ToCallacceptthread(LPVOID p)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ToCallaccept진입"));
	((AnwtestGameModeBase*)p)->acceptthread();
}