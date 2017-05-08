// Fill out your copyright notice in the Description page of Project Settings.

#include "nwtest.h"
#include "Tank.h"
#include "nwtestGameModeBase.h"


using namespace std;



AnwtestGameModeBase::AnwtestGameModeBase()
{
	g_isshutdown = false;
	static ConstructorHelpers::FClassFinder<APawn> GameModeBP(TEXT("Blueprint'/Game/GameMode.GameMode_C'"));
	if (GameModeBP.Class != NULL)
	{
		//DefaultPawnClass = GameModeBP.Class;
		DefaultPawnClass = NULL;
	}
	PrimaryActorTick.bCanEverTick = true;
	threadkey = false;
	SpawnClient = false;
	PlayTick = false;
	SpawnClient = false;
	TankForward = false;
	TankRight = false;
	ForwardSend = false;
	RightSend = false;
	client1_turn = true;
	client2_turn = false;
	StartTimer = false;
	timerTurn = 1;
	moveVal = 0;
	moveVal2 = 0;
	moveVal3 = 0;
	moveVal4 = 0;
	mytime = 20;
}

void AnwtestGameModeBase::BeginPlay()
{	
	Super::BeginPlay();

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	Initialize_Server();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("beforetocallworker"));
	//for (auto i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
	for(auto i = 0; i < 6; ++i)
		worker_threads.push_back(new thread{ &ToCallworkerthread, this });
	for (auto th : worker_threads)
		th->detach();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("beforetocallaccept"));

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("���ϻ�����"));
	accept_threads = new thread{ &ToCallacceptthread, this };
	accept_threads->detach();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("���� ���� ����"));
}

void AnwtestGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("��")));

	threadkey = true;
	closesocket(new_client);
	closesocket(accept_socket);
	for (auto th : worker_threads)
	{
		//th->join();
		//th->detach();
		delete th;
	}
	//accept_threads->detach();
	//accept_threads->join();
	delete accept_threads;

	WSACleanup();
}

//void AnwtestGameModeBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	if (PlayTick == true)
//	{
//		if(room.client1 != nullptr)
//			room.client1->turn = client1_turn;
//		if(room.client2 != nullptr)
//			room.client2->turn = client2_turn;
//		
//		if (SpawnClient == true) // ��ũ ����
//		{
//			GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AnwtestGameModeBase::TimerFunction, 1, true); // Ÿ�̸� ����
//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("���� ���� ����")));
//			Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//			AActor* PlayerStart1 = Cast<AActor>(FindPlayerStart(Test, "1"));
//			AActor*  PlayerStart2 = Cast<AActor>(FindPlayerStart(Test, "2"));
//			UWorld*  World = GetWorld();
//			if (room.client1 != nullptr)
//			{
//				room.client1->m_player.Tank = World->SpawnActor<ATank>(ATank::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
//				room.client1->m_player.Tank->SpawnDefaultController();
//			}
//			if (room.client2 != nullptr)
//			{
//				room.client2->m_player.Tank = World->SpawnActor<ATank>(ATank::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
//				room.client2->m_player.Tank->SpawnDefaultController();
//			}
//			SpawnClient = false;
//		}
//
//		if (TankForward == true) // ��ũ ���� �̵�
//		{
//			
//			if(room.client1 != nullptr)
//				clients[room.client1->m_id].m_player.Tank->SetMoveForward(moveVal);
//			if (room.client2 != nullptr)
//				clients[room.client2->m_id].m_player.Tank->SetMoveForward(moveVal3);
//
//			Tank_Move.size = sizeof(Tank_Move);
//			Tank_Move.type = SC_TANK_MOVE;
//			Tank_Move.location = room.client1->m_player.Tank->GetActorTransform();
//			Tank_Move.location2 = room.client2->m_player.Tank->GetActorTransform();
//
//			if (room.client1 != nullptr)
//				SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));
//			if (room.client2 != nullptr)
//				SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));
//
//			moveVal = 0;
//			moveVal3 = 0;
//			TankForward = false;
//		}
//
//		if (TankRight == true) // ��ũ �¿� �̵�
//		{
//			
//			if (room.client1 != nullptr)
//				clients[room.client1->m_id].m_player.Tank->SetMoveRight(moveVal2);
//			if (room.client2 != nullptr)
//				clients[room.client2->m_id].m_player.Tank->SetMoveRight(moveVal4);
//
//			Tank_Move.size = sizeof(Tank_Move);
//			Tank_Move.type = SC_TANK_MOVE;
//			Tank_Move.location = room.client1->m_player.Tank->GetActorTransform();
//			Tank_Move.location2 = room.client2->m_player.Tank->GetActorTransform();
//		
//			if(room.client1 != nullptr)
//				SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));
//			if(room.client2 != nullptr)
//				SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));
//			
//			moveVal2 = 0;
//			moveVal4 = 0;
//			TankRight = false;
//
//		}
//	}
//}

void AnwtestGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StartTimer == true)
	{
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AnwtestGameModeBase::TimerFunction, 1, true); // Ÿ�̸� ����
		StartTimer = false;
	}
	//if (PlayTick == true)
	//{
	//	/*if (room.client1 != nullptr)
	//		room.client1->turn = client1_turn;
	//	if (room.client2 != nullptr)
	//		room.client2->turn = client2_turn;*/

	//	if (SpawnClient == true) // ��ũ ����
	//	{
	//		//GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AnwtestGameModeBase::TimerFunction, 1, true); // Ÿ�̸� ����
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("���� ���� ����")));
	//		Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//		AActor* PlayerStart1 = Cast<AActor>(FindPlayerStart(Test, "1"));
	//		//AActor*  PlayerStart2 = Cast<AActor>(FindPlayerStart(Test, "2"));
	//		UWorld*  World = GetWorld();
	//		if (room.client1 != nullptr)
	//		{
	//			room.client1->m_player.Tank = World->SpawnActor<ATank>(ATank::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
	//			room.client1->m_player.Tank->SpawnDefaultController();
	//		}
	//	/*	if (room.client2 != nullptr)
	//		{
	//			room.client2->m_player.Tank = World->SpawnActor<ATank>(ATank::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
	//			room.client2->m_player.Tank->SpawnDefaultController();
	//		}*/
	//		SpawnClient = false;
	//	}

	//	if (TankForward == true) // ��ũ ���� �̵�
	//	{
	//		if (room.client1 != nullptr)
	//			clients[room.client1->m_id].m_player.Tank->SetMoveForward(moveVal);
	//		/*if (room.client2 != nullptr)
	//			clients[room.client2->m_id].m_player.Tank->SetMoveForward(moveVal3);*/

	//		Tank_Move.size = sizeof(Tank_Move);
	//		Tank_Move.type = SC_TANK_MOVE;
	//		Tank_Move.worldLocation1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->K2_GetComponentLocation();
	//		Tank_Move.linearVelocity1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->GetPhysicsLinearVelocity();
	//		Tank_Move.angularVelocity1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->GetPhysicsAngularVelocity();
	//		Tank_Move.rotation1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->K2_GetComponentRotation();

	//		
	//		
	//		if(ForwardSend == true)
	//		{
	//			if (room.client1 != nullptr)
	//				clients[room.client1->m_id].m_player.Tank->SetMoveForward(moveVal);
	//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ForwardSend")));
	//			TankForward = false;
	//			/*moveVal = 0;
	//			moveVal3 = 0;*/
	//			if (room.client1 != nullptr)
	//				SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));
	//			ForwardSend = false;
	//		}
	//		/*if (room.client2 != nullptr)
	//			SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));*/
	//	}

	//	if (TankRight == true) // ��ũ �¿� �̵�
	//	{
	//		if (room.client1 != nullptr)
	//			clients[room.client1->m_id].m_player.Tank->SetMoveRight(moveVal2);
	//		/*if (room.client2 != nullptr)
	//			clients[room.client2->m_id].m_player.Tank->SetMoveRight(moveVal4);*/

	//		Tank_Move.size = sizeof(Tank_Move);
	//		Tank_Move.type = SC_TANK_MOVE;
	//		Tank_Move.worldLocation1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->K2_GetComponentLocation();
	//		Tank_Move.linearVelocity1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->GetPhysicsLinearVelocity();
	//		Tank_Move.angularVelocity1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->GetPhysicsAngularVelocity();
	//		Tank_Move.rotation1 = clients[room.client1->m_id].m_player.Tank->GetMesh()->K2_GetComponentRotation();
	//		
	//		if (RightSend == true)
	//		{
	//			if (room.client1 != nullptr)
	//				clients[room.client1->m_id].m_player.Tank->SetMoveRight(moveVal2);
	//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("RightSend")));

	//			TankRight = false;
	//			/*moveVal2 = 0;
	//			moveVal4 = 0;*/
	//			if (room.client1 != nullptr)
	//				SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));
	//			RightSend = false;
	//		}
	//		/*if (room.client2 != nullptr)
	//			SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&Tank_Move));*/

	//	}
	//}
}

void AnwtestGameModeBase::Initialize_Server()
{
	WSADATA   wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	for (auto i = 0; i < MAX_USER; ++i) {
		clients[i].m_recv_overlap.recv_buf.buf =
			reinterpret_cast<CHAR *>(clients[i].m_recv_overlap.socket_buf);
		clients[i].m_recv_overlap.recv_buf.len = MAX_BUF_SIZE;
		clients[i].m_recv_overlap.operation = OP_RECV;
		clients[i].is_connected = false;
	}
	room.state = 0; // 0�̸� ���, 1�̸� 1��, 2�� 2��, 3�̸� ���� ����
	room.counts = 0;
	room.roomNum = 0;
	room.canStart = false;
	room.client1 = nullptr;
	room.client2 = nullptr;
}

void AnwtestGameModeBase::SendPacket(int id, unsigned char *packet)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("SendPacket ����")));
	OverlapEx *send_over = new OverlapEx;
	memset(send_over, 0, sizeof(OverlapEx));
	send_over->operation = OP_SEND;
	send_over->recv_buf.buf = reinterpret_cast<CHAR *>(send_over->socket_buf);
	send_over->recv_buf.len = packet[0];
	memcpy(send_over->socket_buf, packet, packet[0]);
	int res = WSASend(clients[id].m_s, &send_over->recv_buf, 1,
		NULL, 0, &send_over->original_overlap, NULL);

	if (0 != res)
	{
		int error_no = WSAGetLastError();
		if (WSA_IO_PENDING != error_no)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("SendPacket error_no = %d"), error_no));
	}
}

void AnwtestGameModeBase::ProcessPacket(int id, unsigned char *packet)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("ProcessPacket ����")));
	unsigned char packet_type = packet[1];
	switch (packet_type)
	{
	case CS_PLAYER_NAME:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_PLAYER_NAME ����")));
		cs_packet_player_name *player_name = reinterpret_cast<cs_packet_player_name*>(packet);
		
		clients[id].name = player_name->name;

		// �� ���� ����
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Room Show")));
		sc_packet_room_show room_show;
		room_show.counts = room.counts;
		room_show.roomNum = room.roomNum;
		room_show.size = sizeof(room_show);
		room_show.state = room.state;
		room_show.type = SC_ROOM_SHOW;
		SendPacket(id, reinterpret_cast<unsigned char*>(&room_show));
		break;
	}

	case CS_ROOM_CLICK:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_ROOM_CLICK ����")));
		cs_packet_room_click *room_click = reinterpret_cast<cs_packet_room_click*>(packet);
		room.roomNum = room_click->roomNum;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%d"), room.roomNum));
		int playerNum = 0;
		int other_id = -1;
		
		if (room.counts == 0 && room.client1 == nullptr) // 1P�� �濡 ��������
		{
			room.client1 = &clients[id];
			room.client1->playerNum = 1;
			clients[id].state = 2;
			room.state = 1;
			playerNum = 1;
		}
		else if (room.counts == 1 && room.client1 != nullptr) // 2P�� �濡 ��������
		{
			room.client2 = &clients[id];
			room.client2->playerNum = 2;
			clients[id].state = 2;
			room.state = 2;
			playerNum = 2;
			other_id = room.client1->m_id;
		}
		else if (room.counts == 1 && room.client1 == nullptr) // 2P�� ���� �ִ� ���¿��� 1P�� ��������
		{
			room.client1 = &clients[id];
			room.client1->playerNum = 1;
			room.client1->state = 1;
			clients[id].state = 2;
			room.state = 2;
			playerNum = 1;
			other_id = room.client2->m_id;
		}
		else // ���� ó��
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Error : CS_ROOM_CLICK")));
		}
		room.counts++;
		
		// 1. �ϴ� �������� ��P���� �˷��ش�.
		sc_packet_player_num player_num;
		player_num.size = sizeof(player_num);
		player_num.type = SC_PLAYER_NUM;
		player_num.playerNum = playerNum;
		SendPacket(id, reinterpret_cast<unsigned char*>(&player_num));

		// 2. ���� ������� ���� ������ �����ش�.
		sc_packet_room_info room_info;
		room_info.size = sizeof(room_info);
		room_info.type = SC_ROOM_INFO;

		if (playerNum == 1) // ���� ����� 1P�̰�
		{
			if (room.client2 != nullptr) // 2P�� �����Ѵٸ�
			{
				room_info.isReady1 = false;
				room_info.isReady2 = room.client2->isReady;
			}
			else // 2P�� �������� �ʴ´ٸ�
			{
				room_info.isReady1 = false;
				room_info.isReady2 = false;
			}
		}
		if (playerNum == 2) // ���� ����� 2P�̰�
		{
			if (room.client1 != nullptr) // 1P�� �����Ѵٸ�
			{
				room_info.isReady1 = room.client1->isReady;
				room_info.isReady2 = false;
			}
			else // 1P�� �������� �ʴ´ٸ�
			{
				room_info.isReady1 = false;
				room_info.isReady2 = false;
			}
		}

		if (room_info.isReady1 && room_info.isReady2) // 1P�� 2P�� ��� ���� �ߴٸ�
			room_info.canStart = true;
		else room_info.canStart = false;

		room_info.counts = room.counts;

		if (room.client1 != nullptr)
			room_info.name1 = room.client1->name;
		else
			room_info.name1 = "";
		if (room.client2 != nullptr)
			room_info.name2 = room.client2->name;
		else
			room_info.name2 = "";

		SendPacket(id, reinterpret_cast<unsigned char*>(&room_info)); 

		// 2. ���� ���� �濡 �ִ� ������� ��������� �Ѵ�.
		if(other_id != -1) // �ٸ� ����� �ִٸ�.
			SendPacket(other_id, reinterpret_cast<unsigned char*>(&room_info));

		// 3. �κ񿡼� ��ٸ��»�������׵� �ٲ� ���� ���������.
		sc_packet_room_show room_show;
		room_show.size = sizeof(room_show);
		room_show.type = SC_ROOM_SHOW;
		room_show.counts = room.counts;
		room_show.state = room.state;
		room_show.roomNum = room.roomNum;
		for (int i = 0; i < MAX_USER; ++i)
		{
			if (clients[i].is_connected == true && clients[i].state == 1) // ������ �Ǿ��ְ�, �κ� �ִٸ�
			{
				SendPacket(i, reinterpret_cast<unsigned char*>(&room_show));
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_ROOM_CLICK ��")));
		break;
	}

	case CS_READY_CLICK:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_READY_CLICK ����")));
		cs_packet_room_ready *room_ready = reinterpret_cast<cs_packet_room_ready*>(packet);
		int other_id = -1;

		if (clients[id].playerNum == 1 && clients[id].isReady == false) // 1P�� ���������� ó��
		{
			room.client1->isReady = true;
			if(room.client2 != nullptr)
				other_id = room.client2->m_id;
		}
		else if (clients[id].playerNum == 1 && clients[id].isReady == true)
		{
			room.client1->isReady = false;
			if (room.client2 != nullptr)
				other_id = room.client2->m_id;
		}
		else if (clients[id].playerNum == 2 && clients[id].isReady == false) // 2P�� ���������� ó��
		{
			room.client2->isReady = true;
			if (room.client1 != nullptr)
				other_id = room.client1->m_id;
		}
		else if (clients[id].playerNum == 2 && clients[id].isReady == true)
		{
			room.client2->isReady = false;
			if (room.client1 != nullptr)
				other_id = room.client1->m_id;
		}

		if (room.counts == 2) // 2���϶��� canStart ������ ���� �� �����Ƿ�
		{
			if (room.client1->isReady && room.client2->isReady)
				room.canStart = true;
			else
				room.canStart = false;
		}

		sc_packet_room_ready sc_room_ready;
		sc_room_ready.size = sizeof(sc_room_ready);
		sc_room_ready.type = SC_ROOM_READY;
		sc_room_ready.canStart = room.canStart;
		sc_room_ready.isReady1 = false;
		sc_room_ready.isReady2 = false;
		if(room.client1 != nullptr)
			sc_room_ready.isReady1 = room.client1->isReady;
		if(room.client2 != nullptr)
			sc_room_ready.isReady2 = room.client2->isReady;

		// 2�ο�
		// 1. ���� ���� ������� �����ش�.
		SendPacket(id, reinterpret_cast<unsigned char*>(&sc_room_ready));

		//// 2. �������� �����ش�.
		if(other_id != -1)
			SendPacket(other_id, reinterpret_cast<unsigned char*>(&sc_room_ready));

		 //�ӽÿ�
		/*sc_packet_game_start game_start;
		game_start.size = sizeof(game_start);
		game_start.type = SC_GAME_START;
		SendPacket(id, reinterpret_cast<unsigned char*>(&game_start));
		PlayTick = true;
		SpawnClient = true;*/
		
		break;
	}

	case CS_EXIT_CLICK:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_READY_CLICK ����")));
		cs_packet_room_exit *room_exit = reinterpret_cast<cs_packet_room_exit*>(packet);
		int other_id = -1;

		if (clients[id].playerNum == 1) // 1P�� �����ٸ�
		{
			clients[id].state = 1;
			clients[id].isReady = false;
			clients[id].playerNum = 0;
			clients[id].roomNum = -1;
			room.client1 = nullptr;
			if (room.client2 != nullptr) // 2P�� �ִٸ�
				room.state = 1;
			else
				room.state = 0;
			room.canStart = false;
			room.counts--;
			if (room.client2 != nullptr)
				other_id = room.client2->m_id;
		}
		else if (clients[id].playerNum == 2) // 2P�� �����ٸ�
		{
			clients[id].state = 1;
			clients[id].isReady = false;
			clients[id].playerNum = 0;
			clients[id].roomNum = -1;
			room.client2 = nullptr;
			if (room.client1 != nullptr) // 1P�� �ִٸ�
				room.state = 1;
			else
				room.state = 0;
			room.canStart = false;
			room.counts--;
			if (room.client1 != nullptr)
				other_id = room.client1->m_id;
		}

		// 1. ���� �����״� ��ü �� ������ ������� �Ѵ�.
		sc_packet_room_show room_show;
		room_show.counts = room.counts;
		room_show.size = sizeof(room_show);
		room_show.type = SC_ROOM_SHOW;
		room_show.state = room.state;
		room_show.roomNum = room.roomNum;
		SendPacket(id, reinterpret_cast<unsigned char*>(&room_show));

		// 2. �������״� ���� �� ������ ��������� �Ѵ�.
		if (room.counts == 1) // ������ �����ִٸ�
		{
			sc_packet_room_info room_info;
			room_info.size = sizeof(room_info);
			room_info.type = SC_ROOM_INFO;
			room_info.canStart = false;
			room_info.counts = room.counts;
			if (room.client1 != nullptr) // �����ְ� 2P�� 1P�� �����ִٸ�
			{
				room_info.isReady1 = room.client1->isReady;
				room_info.isReady2 = false;
			}
			else if (room.client2 != nullptr)
			{
				room_info.isReady1 = false;
				room_info.isReady2 = room.client2->isReady;
			}
			if (room.client1 != nullptr)
				room_info.name1 = room.client1->name;
			else
				room_info.name1 = "";
			if (room.client2 != nullptr)
				room_info.name2 = room.client2->name;
			else
				room_info.name2 = "";
			SendPacket(other_id, reinterpret_cast<unsigned char*>(&room_info));
		}

		// 3. �κ� �ִ� ��������׵� ��������� �Ѵ�.
		for (int i = 0; i < MAX_USER; ++i)
		{
			if (clients[i].is_connected == true && clients[i].state == 1) // ������ �Ǿ��ְ�, �κ� �ִٸ�
			{
				SendPacket(i, reinterpret_cast<unsigned char*>(&room_show));
			}
		}
		break;
	}

	case CS_GAME_START:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_GAME_START ����")));
		
		if (room.client1 != nullptr)
			clients[room.client1->m_id].turn = false;
			//clients[room.client1->m_id].turn = true;
		if (room.client2 != nullptr)
			clients[room.client2->m_id].turn = false;
		if (room.client1 != nullptr)
			clients[room.client1->m_id].state = 3;
		if (room.client2 != nullptr)
			clients[room.client2->m_id].state = 3;
		room.state = 3;
		sc_packet_game_start game_start;
		game_start.size = sizeof(game_start);
		game_start.type = SC_GAME_START;
		//game_start.turn = true;
		game_start.turn = false;
		// 1. ������ �����϶�� �����ش�.
		if (room.client1 != nullptr)
			SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&game_start));
		if (room.client2 != nullptr)
			SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&game_start));

		// ���������� ��ũ�� �������ش�. 
		/*PlayTick = true;
		SpawnClient = true;*/
		break;
	}
	case CS_FINISH_LOAD:
	{
		int other_id = -1;
		if (clients[id].playerNum == 1)
		{
			clients[id].isLoad = true;
			if (room.client2 != nullptr)
				other_id = room.client2->m_id;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("1P �ε� �Ϸ�")));
		}
		else if (clients[id].playerNum == 2)
		{
			clients[id].isLoad = true;
			if (room.client1 != nullptr)
				other_id = room.client1->m_id;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("2P �ε� �Ϸ�")));
		}
		
		if (room.client1 != nullptr && room.client2 != nullptr)
		{
			if (room.client1->isLoad == true && room.client2->isLoad == true)
			{
				clients[room.client1->m_id].turn = true;
				clients[room.client2->m_id].turn = false;
				StartTimer = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("��� �ε� �Ϸ�")));
				sc_packet_turn packet_turn;
				packet_turn.size = sizeof(packet_turn);
				packet_turn.type = SC_TURN;
				packet_turn.turn = true;
				if(room.client1 != nullptr)
					SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&packet_turn));
				packet_turn.turn = false;
				if(room.client2 != nullptr)
					SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&packet_turn));
			}
		}

		break;
	}
	case CS_PLAYER_MOVE:
	{
		cs_packet_player_move *player_move = reinterpret_cast<cs_packet_player_move*>(packet);
		int other_id = -1;
		if (clients[id].playerNum == 1)
		{
			if (room.client2 != nullptr)
				other_id = room.client2->m_id;
		}
		else if (clients[id].playerNum == 2)
		{
			if(room.client1 != nullptr)
				other_id = room.client1->m_id;
		}
		
		sc_packet_player_move sc_player_move;
		sc_player_move.size = sizeof(sc_player_move);
		sc_player_move.type = SC_PLAYER_MOVE;
		sc_player_move.worldLocation = player_move->worldLocation;
		sc_player_move.linearVelocity = player_move->linearVelocity;
		sc_player_move.angularVelocity = player_move->angularVelocity;
		sc_player_move.rotation = player_move->rotation;
		sc_player_move.droneLocation = player_move->droneLocation;
		sc_player_move.droneRotation = player_move->droneRotation;
		
		SendPacket(other_id, reinterpret_cast<unsigned char*>(&sc_player_move));
		break;
	}

	case CS_TANK_SHOT:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("��ź �߻�")));
		cs_packet_tank_shot *tank_shot = reinterpret_cast<cs_packet_tank_shot*>(packet);
		int other_id = -1;
		if (clients[id].playerNum == 1)
		{
			if (room.client2 != nullptr)
				other_id = room.client2->m_id;
		}
		else if (clients[id].playerNum == 2)
		{
			if (room.client1 != nullptr)
				other_id = room.client1->m_id;
		}

		sc_packet_tank_shot sc_tank_shot;
		sc_tank_shot.size = sizeof(sc_tank_shot);
		sc_tank_shot.type = SC_TANK_SHOT;
		sc_tank_shot.location = tank_shot->location;
		sc_tank_shot.rotation = tank_shot->rotation;
		sc_tank_shot.power = tank_shot->power;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f, %f, %f"), sc_tank_shot.location.X, sc_tank_shot.location.Y, sc_tank_shot.location.Z));
		SendPacket(other_id, reinterpret_cast<unsigned char*>(&sc_tank_shot));
		break;
	}
	case CS_TANK_HIT:
	{
		cs_packet_tank_hit *tank_hit = reinterpret_cast<cs_packet_tank_hit*>(packet);
		int other_id = -1;
		
		sc_packet_tank_hit sc_tank_hit;
		sc_tank_hit.size = sizeof(sc_tank_hit);
		sc_tank_hit.type = SC_TANK_HIT;
		
		if (clients[id].playerNum == 1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("1P ��ź ����")));
			if (room.client2 != nullptr)
				other_id = room.client2->m_id;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("���� ������ : %f "), tank_hit->damage));
			clients[id].hp -= tank_hit->damage;
			sc_tank_hit.hp = clients[id].hp;
			sc_tank_hit.shield = clients[id].shield;
			sc_tank_hit.playerNum = clients[id].playerNum;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("���� HP : %f "), clients[id].hp));
		}
		else if (clients[id].playerNum == 2)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("2P ��ź ����")));
			if (room.client1 != nullptr)
				other_id = room.client1->m_id;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("���� ������ : %f "), tank_hit->damage));
			clients[id].hp -= tank_hit->damage;
			sc_tank_hit.hp = clients[id].hp;
			sc_tank_hit.shield = clients[id].shield;
			sc_tank_hit.playerNum = clients[id].playerNum;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("���� HP : %f "), clients[id].hp));
		}

		SendPacket(id, reinterpret_cast<unsigned char*>(&sc_tank_hit));
		SendPacket(other_id, reinterpret_cast<unsigned char*>(&sc_tank_hit));
		break;
	}
	/*case CS_PRESS_FORWARD:
	{
		cs_packet_press_move *press_forward = reinterpret_cast<cs_packet_press_move*>(packet);
		moveVal = press_forward->val;
		TankForward = true;
		break;
	}
	case CS_PRESS_RIGHT:
	{
		cs_packet_press_move *press_right = reinterpret_cast<cs_packet_press_move*>(packet);
		moveVal2 = press_right->val;
		TankRight = true;
		break;
	}
	case CS_RELEASE_FORWARD:
	{
		cs_packet_release_move *release_forward = reinterpret_cast<cs_packet_release_move*>(packet);
		moveVal = release_forward->val;
		ForwardSend = true;
		break;
	}
	case CS_RELEASE_RIGHT:
	{
		cs_packet_release_move *release_right = reinterpret_cast<cs_packet_release_move*>(packet);
		moveVal2 = release_right->val;
		RightSend = true;
		break;
	}*/
	//case CS_TANK_FORWARD:
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_TANK_FORWARD ����")));
	//	cs_packet_tank_move *tank_move = reinterpret_cast<cs_packet_tank_move*>(packet);
	//	//int other_id = -1;
	//	if (clients[id].playerNum == 1)
	//	{
	//		//if(room.client2 != nullptr)
	//			//other_id = room.client2->m_id;
	//		moveVal = tank_move->val;
	//	}
	//	if (clients[id].playerNum == 2)
	//	{
	//		//if(room.client1 != nullptr)
	//			//other_id = room.client1->m_id;
	//		moveVal3 = tank_move->val;
	//	}

	//	TankForward = true;

	//	break;
	//}
	//case CS_TANK_RIGHT:
	//{
	//	cs_packet_tank_move *tank_move = reinterpret_cast<cs_packet_tank_move*>(packet);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("CS_TANK_RIGHT ����")));
	//	//int other_id = -1;
	//	if (clients[id].playerNum == 1)
	//	{
	//		//if(room.client2 != nullptr)
	//		//	other_id = room.client2->m_id;
	//		moveVal2 = tank_move->val;
	//	}
	//	if (clients[id].playerNum == 2)
	//	{
	//		//if(room.client1 != nullptr)
	//			//other_id = room.client1->m_id;
	//		moveVal4 = tank_move->val;
	//	}
	//	TankRight = true;
	//	break;
	//}
	default:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("�˼����� ��Ŷ")));
		break;
	}
	}
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

		//if (false == result) { // Ŭ���̾�Ʈ�� ���������� ���
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WorkerThread - false == result")));
		//	 //Error�� ó���Ѵ�.
		//	//int err_no = WSAGetLastError();
		//	//if (64 == err_no) {
		//	//	/*closesocket(room.clients[key].m_s);
		//	//	room.clients[key].is_connected = false;*/
		//	//	closesocket(clients[key].m_s);
		//	//	clients[key].is_connected = false;
		//	//}
		//	//else
		//	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("GQCS Error")));
		//}

		if (result == false || io_size == 0) // Ŭ���̾�Ʈ�� closesocket()���� ���������� ���
		{
			if (result == false) // Ŭ���̾�Ʈ�� ���������� ���
			{
				int error_no = WSAGetLastError();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Ŭ���̾�Ʈ �������� %d"), error_no));
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WorkerThread - io_size == 0")));
			closesocket(clients[key].m_s);
			clients[key].is_connected = false;
			int other_id = -1;

			if (clients[key].playerNum == 1) // 1P�� �����ٸ�
			{
				clients[key].state = 0;
				clients[key].isReady = false;
				clients[key].playerNum = 0;
				clients[key].roomNum = -1;
				clients[key].isLoad = false;
				clients[key].name = "";
				clients[key].turn = false;
				clients[key].m_player.Tank = nullptr; // ��ũ ���۷��� �����ش�.
				room.client1 = nullptr;
				if (room.client2 != nullptr) // 2P�� �ִٸ�
					room.state = 1;
				else
					room.state = 0;
				room.canStart = false;
				room.counts--;
				if (room.client2 != nullptr)
					other_id = room.client2->m_id;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("1P�� ������, ����ó���� �Ͽ���")));
			}
			else if (clients[key].playerNum == 2) // 2P�� �����ٸ�
			{
				clients[key].state = 0;
				clients[key].isReady = false;
				clients[key].playerNum = 0;
				clients[key].roomNum = -1;
				clients[key].isLoad = false;
				clients[key].name = "";
				clients[key].turn = false;
				clients[key].m_player.Tank = nullptr; // ��ũ ���۷��� �����ش�.
				room.client2 = nullptr;
				if (room.client1 != nullptr) // 1P�� �ִٸ�
					room.state = 1;
				else
					room.state = 0;
				room.canStart = false;
				room.counts--;
				if (room.client1 != nullptr)
					other_id = room.client1->m_id;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("2P�� ������, ����ó���� �Ͽ���")));
			}

			// 1. �������״� ���� �� ������ ��������� �Ѵ�.
			// �������� ���¿��� ������ ������ ��� �ؾ����� ���� �ؾ� ��.
			if (room.counts == 1) // ������ �����ִٸ�
			{
				sc_packet_room_info room_info;
				room_info.size = sizeof(room_info);
				room_info.type = SC_ROOM_INFO;
				room_info.canStart = false;
				room_info.counts = room.counts;
				if (room.client1 != nullptr) // �����ְ� 2P�� 1P�� �����ִٸ�
				{
					room_info.isReady1 = room.client1->isReady;
					room_info.isReady2 = false;
				}
				else if (room.client2 != nullptr)
				{
					room_info.isReady1 = false;
					room_info.isReady2 = room.client2->isReady;
				}
				if (room.client1 != nullptr)
					room_info.name1 = room.client1->name;
				else
					room_info.name1 = "";
				if (room.client2 != nullptr)
					room_info.name2 = room.client2->name;
				else
					room_info.name2 = "";

				// ������ ���濡 ������, ���� ������ �����ش�.
				if (clients[other_id].state == 2)
				{
					SendPacket(other_id, reinterpret_cast<unsigned char*>(&room_info));
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("1P�� ������, 2P���� �������� ������")));
				}
				// ������ �������̿��ٸ� �κ�� ���ư���� ��Ŷ�� �����ش�.
				if (clients[other_id].state == 3)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("1P�� ������, 2P���� �κ�� ���ư���� ���������.")));
					//clients[other_id].state = 1; // Ŭ���̾�Ʈ ���¸� �κ�� �ٲ۴�. ���� �ؿ� 2�� �������� ������ �� ���Ž�����.
					//room.counts--;
					//room.canStart = false;
					//room.client1 = nullptr;
					//room.client2 = nullptr;
					//room.state = 0;
					//sc_packet_finish_game finish_game;
					//finish_game.size = sizeof(finish_game);
					//finish_game.type = SC_FINISH_GAME;
					//SendPacket(other_id, reinterpret_cast<unsigned char*>(&finish_game));
				}
			}

			// 2. �κ� �ִ� ��������׵� ��������� �Ѵ�.
			for (int i = 0; i < MAX_USER; ++i)
			{
				if (clients[i].is_connected == true && clients[i].state == 1) // ������ �Ǿ��ְ�, �κ� �ִٸ�
				{
					sc_packet_room_show room_show;
					room_show.counts = room.counts;
					room_show.size = sizeof(room_show);
					room_show.type = SC_ROOM_SHOW;
					room_show.state = room.state;
					room_show.roomNum = room.roomNum;
					SendPacket(i, reinterpret_cast<unsigned char*>(&room_show));
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("�κ� �ִ� ������Ե� ��������")));
				}
			}
			
			continue;
		}

		if (overlap->operation == OP_RECV)
		{
			unsigned char *buf_ptr = overlap->socket_buf;
			int remained = io_size;

			while (0 < remained) { // ��Ŷ�� �ٿö����� ó���ؾ���.
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
				
				DWORD flags = 0;
				WSARecv(clients[key].m_s,
					&clients[key].m_recv_overlap.recv_buf,
					1, NULL, &flags,
					reinterpret_cast<LPWSAOVERLAPPED>(&clients[key].m_recv_overlap),
					NULL);
			}
		}
		else if (overlap->operation == OP_SEND)
		{
			delete overlap;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Unknown Event on Worker_Thread")));
			//while (true);
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
		clients[new_id].isReady = false;
		clients[new_id].state = 1;
		clients[new_id].playerNum = 0;
		clients[new_id].name = "temp";
		clients[new_id].roomNum = -1;
		clients[new_id].m_player.Tank = nullptr;
		clients[new_id].turn = false;
		clients[new_id].isLoad = false;
		clients[new_id].hp = 100;
		clients[new_id].shield = 100;
		//clients[new_id].worldLocation = FVector(0.f, 0.f, 0.f);
		//clients[new_id].linearVelocity = FVector(0.f, 0.f, 0.f);
		//clients[new_id].angularVelocity = FVector(0.f, 0.f, 0.f);
		//clients[new_id].rotation = FRotator(0.f, 0.f, 0.f);

		ZeroMemory(&clients[new_id].m_recv_overlap, sizeof(OVERLAPPED));
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Accept - Recv")));
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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ToCallaccept����"));
	((AnwtestGameModeBase*)p)->acceptthread();
}

void AnwtestGameModeBase::SpawnPlayer(int id)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SpawnPlayer"));

	APlayerController*  Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AActor* PlayerStart1 = Cast<AActor>(FindPlayerStart(Test, "1"));
	AActor*  PlayerStart2 = Cast<AActor>(FindPlayerStart(Test, "2"));

	UWorld*  World = GetWorld();

	clients[0].m_player.Tank = World->SpawnActor<ATank>(ATank::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
*/
	//MyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart1->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart1->GetActorRotation());

	//EnemyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
	//EnemyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticCl
}

void AnwtestGameModeBase::TimerFunction()
{
	// Ÿ�̸� ����
	sc_packet_timer packet_timer;
	packet_timer.type = SC_TIMER;
	packet_timer.size = sizeof(packet_timer);
	packet_timer.timer = mytime;
	if (room.client1 != nullptr)
		SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&packet_timer));
	if (room.client2 != nullptr)
		SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&packet_timer));

	mytime--;
	
	if (mytime <= 0) {
		if (timerTurn == 1) // 1P���ٸ� 2P�� ����
		{
			if (room.client1 != nullptr)
				room.client1->turn = false;
			if (room.client2 != nullptr)
				room.client2->turn = true;
			timerTurn = 2;
			// ���� �ٲ���ٰ� �˷������.
			sc_packet_turn packet_turn;
			packet_turn.size = sizeof(packet_turn);
			packet_turn.type = SC_TURN;
			packet_turn.turn = false;
			packet_turn.ap = 0;
			if(room.client1 != nullptr)
				SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&packet_turn)); // 1P���״� false��
			packet_turn.turn = true;
			packet_turn.ap = 1500;
			if(room.client2 != nullptr)
				SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&packet_turn)); // 2P���״� true��, AP 500������
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SC_TURN ������"));
		}
		else // 2P���ٸ� 1P�� ����
		{
			timerTurn = 1;
			if (room.client1 != nullptr)
				room.client1->turn = true;
			if (room.client2 != nullptr)
				room.client2->turn = false;
			// ���� �ٲ���ٰ� �˷������.
			sc_packet_turn packet_turn;
			packet_turn.size = sizeof(packet_turn);
			packet_turn.type = SC_TURN;
			packet_turn.turn = true;
			packet_turn.ap = 1500;
			if (room.client1 != nullptr)
				SendPacket(room.client1->m_id, reinterpret_cast<unsigned char*>(&packet_turn)); // 1P���״� true��, AP 500 ������
			packet_turn.turn = false;
			packet_turn.ap = 0;
			if (room.client2 != nullptr)
				SendPacket(room.client2->m_id, reinterpret_cast<unsigned char*>(&packet_turn)); // 2P���״� false��
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SC_TURN ������"));
		}
		mytime = 20;
	}
}
