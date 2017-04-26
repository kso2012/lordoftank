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
	/*for (auto i = 0; i < MAX_USER; ++i) {
		room.clients[i].m_recv_overlap.recv_buf.buf =
			reinterpret_cast<CHAR *>(room.clients[i].m_recv_overlap.socket_buf);
		room.clients[i].m_recv_overlap.recv_buf.len = MAX_BUF_SIZE;
		room.clients[i].m_recv_overlap.operation = OP_RECV;
		room.clients[i].is_connected = false;
	}*/
	room.state = 0; // 0이면 빈방, 1이면 1명, 2면 2명, 3이면 게임 시작
	room.counts = 0;
	room.roomNum = 0;
	//room.canStart = false;
	room.client1 = nullptr;
	room.client2 = nullptr;
	
}
void AnwtestGameModeBase::SendPacket(int id, unsigned char *packet)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("SendPacket 진입")));
	OverlapEx *send_over = new OverlapEx;
	memset(send_over, 0, sizeof(OverlapEx));
	send_over->operation = OP_SEND;
	send_over->recv_buf.buf = reinterpret_cast<CHAR *>(send_over->socket_buf);
	send_over->recv_buf.len = packet[0];
	memcpy(send_over->socket_buf, packet, packet[0]);
	int res = WSASend(clients[id].m_s, &send_over->recv_buf, 1,
		NULL, 0, &send_over->original_overlap, NULL);
	//int res = WSASend(room.clients[id].m_s, &send_over->recv_buf, 1,
		//NULL, 0, &send_over->original_overlap, NULL);
	if (0 != res)
	{
		int error_no = WSAGetLastError();
		if (WSA_IO_PENDING != error_no)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("SendPacket error_no = %d"), error_no));
			//error_display("SendPacket:WSASend", error_no);
	}
}

void AnwtestGameModeBase::ProcessPacket(int id, unsigned char *packet)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ProcessPacket 진입")));
	
	unsigned char packet_type = packet[1];
	switch (packet_type)
	{
	case CS_ROOM_CLICK:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_ROOM_CLICK 진입")));
		cs_packet_room_click *cs_room_click = reinterpret_cast<cs_packet_room_click*>(packet);
		room.roomNum = cs_room_click->roomNum;
		int playerNum;
		
		if (room.counts == 0 && room.client1 == NULL) // 1P로 방에 들어왔을때
		{
			room.client1 = &clients[id];
			room.state = 1;
			playerNum = 1;
		}
		else if (room.counts == 1 && room.client1 != NULL) // 2P로 방에 들어왔을때
		{
			room.client2 = &clients[id];
			room.state = 2;
			playerNum = 2;
		}
		room.counts++;

		sc_packet_room_info room_info;
		room_info.size = sizeof(room_info);
		room_info.type = SC_ROOM_INFO;
		room_info.isReady1 = false;
		room_info.isReady2 = false;
		if (room.client1->isReady && room.client2->isReady)
			room_info.canStart = true;
		else
			room_info.canStart = false;
		room_info.playerNum = playerNum;
		room_info.counts = room.counts;
		//room_info.name1 = room.client1->name;
		//room_info.name2 = room.client2->name;
		
		room_info.name1 = "temp1";
		room_info.name2 = "temp2";

		//for(int i=0; i<MAX_USER; ++i)
		SendPacket(id, reinterpret_cast<unsigned char*>(&room_info)); // 바뀐 방의 정보를 보내준다.

		// 나중에 방 늘어나면, 로비에서 기다리는사람들한테도 바뀐 것을 보내줘야함.

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_ROOM_CLICK 끝")));
		break;
	}
	case CS_FORWARD:
		break;
	case CS_RIGHT:
		break;
	default:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("알수없는 패킷")));
		//cs_packet_room_click *error = reinterpret_cast<cs_packet_room_click*>(packet);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT(" TYPE : %d"),error->type ));
		//break;
	}
	}

	//Packet *p = reinterpret_cast<Packet*>(packet);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("X = %f, Y = %f, Z = %f"), p->Pos.X, p->Pos.Y, p->Pos.Z));

	//clients[id].m_player.VehicleMovement->SetThrottleInput(1.0f);



	/*sc_packet_tank_move tank_move;
	tank_move.id = id;
	tank_move.size = sizeof(sc_packet_tank_move);
	tank_move.num = 100;

	SendPacket(id, reinterpret_cast<unsigned char*>(&tank_move));*/

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

		if (false == result) { // 클라이언트가 강제종료한 경우
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WorkerThread - false == result")));
			 //Error를 처리한다.
			//int err_no = WSAGetLastError();
			//if (64 == err_no) {
			//	/*closesocket(room.clients[key].m_s);
			//	room.clients[key].is_connected = false;*/
			//	closesocket(clients[key].m_s);
			//	clients[key].is_connected = false;
			//}
			//else
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("GQCS Error")));
		}

		if (io_size == 0) // 클라이언트가 closesocket()으로 정상종료한 경우
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WorkerThread - io_size == 0")));
			/*closesocket(room.clients[key].m_s);
			room.clients[key].is_connected = false;*/
			closesocket(clients[key].m_s);
			clients[key].is_connected = false;
			continue;
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
				/*if (0 == room.clients[key].m_recv_overlap.packet_size) {
					room.clients[key].m_recv_overlap.packet_size = buf_ptr[0];
				}
				int required = room.clients[key].m_recv_overlap.packet_size
					- room.clients[key].previous_data;
				if (remained >= required) {
					memcpy(room.clients[key].packet + room.clients[key].previous_data,
						buf_ptr,
						required);
					ProcessPacket(key, room.clients[key].packet);
					remained -= required;
					buf_ptr += required;
					room.clients[key].m_recv_overlap.packet_size = 0;
					room.clients[key].previous_data = 0;
				}
				else {
					memcpy(room.clients[key].packet + room.clients[key].previous_data,
						buf_ptr, remained);
					room.clients[key].previous_data += remained;
					remained = 0;
					buf_ptr += remained;
				}
			}*/
				


				DWORD flags = 0;
				WSARecv(clients[key].m_s,
					&clients[key].m_recv_overlap.recv_buf,
					1, NULL, &flags,
					reinterpret_cast<LPWSAOVERLAPPED>(&clients[key].m_recv_overlap),
					NULL);
				/*WSARecv(room.clients[key].m_s,
					&room.clients[key].m_recv_overlap.recv_buf,
					1, NULL, &flags,
					reinterpret_cast<LPWSAOVERLAPPED>(&room.clients[key].m_recv_overlap),
					NULL);*/
			}
		}
		else if (overlap->operation == OP_SEND)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WorkerThread - SEND")));
			delete overlap;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Unknown Event on Worker_Thread")));
			//cout << "Unknown Event on worker_thread" << endl;
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
		/*for (auto i = 0; i < MAX_USER; ++i) {
			if (room.clients[i].is_connected == false) {
				new_id = i;
				break;
			}
		}*/

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
		ZeroMemory(&clients[new_id].m_recv_overlap, sizeof(OVERLAPPED));
		/*room.clients[new_id].m_s = new_client;
		room.clients[new_id].is_connected = true;
		room.clients[new_id].m_id = new_id;
		room.clients[new_id].m_recv_overlap.operation = OP_RECV;
		room.clients[new_id].m_recv_overlap.packet_size = 0;
		room.clients[new_id].previous_data = 0;*/

		// 방 정보 전송
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Room Show")));
		sc_packet_room_show room_show;
		//room_show.canStart = room.canStart;
		room_show.counts = room.counts;
		room_show.roomNum = room.roomNum;
		room_show.size = sizeof(room_show);
		room_show.state = room.state;
		room_show.type = SC_ROOM_SHOW;
		SendPacket(new_id, reinterpret_cast<unsigned char*>(&room_show));

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Accept - Recv")));
		DWORD flags = 0;
		int res = WSARecv(new_client,
			&clients[new_id].m_recv_overlap.recv_buf, 1, NULL,
			&flags,
			&clients[new_id].m_recv_overlap.original_overlap, NULL);
		/*DWORD flags = 0;
		int res = WSARecv(new_client,
			&room.clients[new_id].m_recv_overlap.recv_buf, 1, NULL,
			&flags,
			&room.clients[new_id].m_recv_overlap.original_overlap, NULL);*/


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