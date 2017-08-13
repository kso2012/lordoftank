// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "StartGameMode.h"
#include "GameMode/MultiGameMode.h"
#include "LOTGameInstance.h"




ULOTGameInstance::ULOTGameInstance()
{
	bIsConnected = false;
	bIsStart = false;
	bIsHurt = false;
	PlayerNum = 0;
	HitEmpPlayerNum = 0;
	LeftTime = 0;
	GameStateEnum = EGameState::Mode_Main;
	bEnemyIsShot = false;
	bChangeTurnMS = false;
	bIsWaiting = true;
	bRecvIsEndGame = false;
	bIsTargetMS = false;
	bRecvHitMS = false;
	bIsTarget = false;
	bIsHitEmp = false;
	bRecvBeamMS = false;
	bBeamIsActivated = false;
	EndState = 0;
	RoomInfo.SetNum(5);
	bIsCreatingItem = false;
	bIsPlaneAlive = false;

	itemIndex = -1;

	bAteHP = false;
	bAteSHIELD = false;
	bAteAP = false;
	bAte_ARMOR_PIERCE = false;
	bAte_HOMING = false;
	bAte_EMP = false;
}

void ULOTGameInstance::ResetVar()
{
	bIsStart = false;
	bIsHurt = false;
	bEnemyIsShot = false;
	bChangeTurnMS = false;
	bIsWaiting = true;
	bRecvIsEndGame = false;
	PlayerNum = 0;
	LeftTime = 0;
	HitEmpPlayerNum = 0;
	bIsTargetMS = false;
	bRecvHitMS = false;
	bIsTarget = false;
	bIsHitEmp = false;
	GameStateEnum = EGameState::Mode_Main;
	EndState = 0;
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
	serveraddr.sin_addr.s_addr = inet_addr("175.196.145.229");
	//serveraddr.sin_addr.s_addr = inet_addr(TCHAR_TO_UTF8(*IPaddr));
	serveraddr.sin_port = htons(SERVER_PORT);

	InitEvent(sock);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	int error_code = WSAGetLastError();
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), error_code));
	if (retval == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 성공"));
		WSAEventSelect(app.sarr[app.now], app.hEarr[app.now], FD_CONNECT);
		app.now++;
		// 스레드 생성
		event_thread = new thread{ &ToCalleventThread, this };
		bIsConnected = true;
		return true;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("커넥트 실패"));
		//bIsTryConnecting = false;
		bIsConnected = false;
		closesocket(sock);
		WSACleanup();
		return false;
	}

	

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
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("방정보받음")));
		sc_packet_room_show *my_packet = reinterpret_cast<sc_packet_room_show*>(ptr);
		for (int i = 0; i < MAX_ROOM; ++i) {
			RoomInfo[i].roomNum = my_packet->roomNum[i];
			RoomInfo[i].counts = my_packet->counts[i];
			RoomInfo[i].state = my_packet->state[i];
		}
		break;
	}
	case SC_PLAYER_NUM:
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("플레이어넘버받음")));
		sc_packet_player_num *my_packet = reinterpret_cast<sc_packet_player_num*>(ptr);
		PlayerNum = my_packet->playerNum;
		break;
	}
	case SC_ROOM_INFO:
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("로비정보받음")));
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("레디정보받음")));
		sc_packet_room_ready *my_packet = reinterpret_cast<sc_packet_room_ready*>(ptr);
		LobbyInfo.canStart = my_packet->canStart;
		LobbyInfo.isReady1 = my_packet->isReady1;
		LobbyInfo.isReady2 = my_packet->isReady2;
		break;
	}

	case SC_GAME_START:
	{
		sc_packet_game_start *my_packet = reinterpret_cast<sc_packet_game_start*>(ptr);
		GameStateEnum = EGameState::Mode_Multiplay;
		bIsStart = true;
		bIsmyTurn = my_packet->turn;
		break;
	}
	case SC_TANK_HIT:
	{
		sc_packet_tank_hit *my_packet = reinterpret_cast<sc_packet_tank_hit*>(ptr);
		bIsHurt = true;
		HP = my_packet->hp;
		Shield = my_packet->shield;
		HitPlayerNum = my_packet->playerNum;

		break;
	}
	case SC_TURN:
	{
		sc_packet_turn *my_packet = reinterpret_cast<sc_packet_turn*>(ptr);
		bChangeTurnMS = true;
		bIsmyTurn = my_packet->turn;
		ChargingAP = my_packet->ap;
		//bUseableDrone = my_packet->canUseDrone;
		break;
	}

	case SC_TIMER:
	{
		sc_packet_timer *my_packet = reinterpret_cast<sc_packet_timer*>(ptr);
		bIsWaiting = my_packet->isWaiting;
		LeftTime = my_packet->timer;
		break;
	}
	case SC_PLAYER_MOVE:
	{
		
		sc_packet_player_move *my_packet = reinterpret_cast<sc_packet_player_move*>(ptr);

		EnemyWorldLocation = my_packet->worldLocation;
		EnemyLinearVelocity = my_packet->linearVelocity;
		EnemyAngularVelocity = my_packet->angularVelocity;
		EnemyRotation = my_packet->rotation;
		EnemyDroneLocation = my_packet->droneLocation;
		EnemyDroneRotation = my_packet->droneRotation;

		break;
	}
	case SC_TANK_SHOT:
	{
		sc_packet_tank_shot *my_packet = reinterpret_cast<sc_packet_tank_shot*>(ptr);
		EnemyShotLocation = my_packet->location;
		EnemyShotPower = my_packet->power;
		EnemyShotRotation = my_packet->rotation;
		EnemyShotProjectileType = my_packet->projectile;
		bEnemyIsShot = true;
		
		break;
	}

	case SC_DRONE_TARGETING:
	{
		sc_packet_drone_targeting *my_packet = reinterpret_cast<sc_packet_drone_targeting*>(ptr);
		bIsTargetMS = true;
		bIsTarget = my_packet->isTargeting;
		break;
	}

	case SC_ACTIVATE_HOMING:
	{
		sc_packet_activate_homing *my_packet = reinterpret_cast<sc_packet_activate_homing*>(ptr);
		bEnemyActivateHoming = true;
		break;

	}
	case SC_FINISH_GAME:
	{
		sc_packet_finish_game *my_packet = reinterpret_cast<sc_packet_finish_game*>(ptr);
		EndState = my_packet->state;
		bRecvIsEndGame = true;

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("상대 쳐나감")));
		
		break;
	}

	case SC_EMP_ON:
	{
		sc_packet_emp *my_packet = reinterpret_cast<sc_packet_emp*>(ptr);
		HitEmpPlayerNum = my_packet->playerNum;
		bIsHitEmp = true;
		bRecvHitMS = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("emp on 받음")));
		break;
	}

	case SC_EMP_OFF:
	{
		sc_packet_emp *my_packet = reinterpret_cast<sc_packet_emp*>(ptr);
		HitEmpPlayerNum = my_packet->playerNum;
		bIsHitEmp = false;
		bRecvHitMS = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("emp off 받음")));
		break;
	}

	case SC_BEAM_ON:
	{
		sc_packet_use_beam *my_packet = reinterpret_cast<sc_packet_use_beam*>(ptr);
		bRecvBeamMS = true;
		bBeamIsActivated = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("beam on 받음")));
		break;
	}

	case SC_BEAM_OFF:
	{
		sc_packet_use_beam *my_packet = reinterpret_cast<sc_packet_use_beam*>(ptr);
		bRecvBeamMS = true;
		bBeamIsActivated = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("beam off 받음")));
		break;
	}

	case SC_SPAWN_PLANE:
	{
		sc_packet_spawn_plane *my_packet = reinterpret_cast<sc_packet_spawn_plane*>(ptr);
		planeSpawnLocation = my_packet->planeLocation;
		itemSpawnLocation = my_packet->itemLocation;
		D_itemNum = my_packet->itemNum;
		bIsPlaneAlive = true;
		itemIndex++;
		ItemsInfo[itemIndex].itemNum = D_itemNum;
		break;
	}

	case SC_PLANE_MOVE:
	{
		sc_packet_plane_move *my_packet = reinterpret_cast<sc_packet_plane_move*>(ptr);
		P_Location = my_packet->location;
		P_Rotation = my_packet->rotator;
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString::Printf(TEXT("비행기 좌표 %f %f %f"), P_Location.X, P_Location.Y, P_Location.Z));
		break;
	}

	case SC_SPAWN_ITEM:
	{
		bIsCreatingItem = true;
		ItemsInfo[itemIndex].bIsAlive = true;
		break;
	}

	case SC_ITEM_MOVE:
	{
		sc_packet_item_move *my_packet = reinterpret_cast<sc_packet_item_move*>(ptr);
		ItemsInfo[my_packet->index].item_Location = my_packet->location;
		ItemsInfo[my_packet->index].item_Rotation = my_packet->rotator;
		Parachute_Size = my_packet->parachuteSize;
		//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Blue, FString::Printf(TEXT("아이템 좌표 %f %f %f"), I_Location.X, I_Location.Y, I_Location.Z));
		break;
	}

	case SC_DELETE_PLANE:
	{
		bIsPlaneAlive = false;
		//GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Yellow, FString::Printf(TEXT("비행기 삭제 메시지 받음")));
		break;
	}

	case SC_EAT_ITEM:
	{
		sc_packet_eat_item *my_packet = reinterpret_cast<sc_packet_eat_item*>(ptr);
		E_itemNum = my_packet->itemNum;
		ItemsInfo[E_itemNum].bIsAlive = false;
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString::Printf(TEXT("%d번 아이템을 먹었다고 받음"), E_itemNum));
		break;
	}

	case SC_RESTORE_HP:
	{
		sc_packet_adjust_item *my_packet = reinterpret_cast<sc_packet_adjust_item*>(ptr);
		bAteHP = true;
		delta_HP = my_packet->num;
		break;
	}

	case SC_RESTORE_SHIELD:
	{
		sc_packet_adjust_item *my_packet = reinterpret_cast<sc_packet_adjust_item*>(ptr);
		bAteSHIELD = true;
		delta_SHIELD = my_packet->num;
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
	if (RoomInfo[roomnum].state == 0 || RoomInfo[roomnum].state == 1)
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

	if (bIsConnected) {
		closesocket(sock);
		event_thread->detach();
		delete(event_thread);
		WSACleanup();
	}

}


void ULOTGameInstance::SendLocationInfo(FVector LinearVel, FVector AngularVel, FVector Location, FRotator Rotation,FVector DroneLocation,FRotator DroneRotation)
{
	cs_packet_player_move *player_move = reinterpret_cast<cs_packet_player_move*>(send_buffer);
	player_move->linearVelocity = LinearVel;
	player_move->angularVelocity = AngularVel;
	player_move->worldLocation = Location;
	player_move->rotation = Rotation;
	player_move->droneLocation = DroneLocation;
	player_move->droneRotation = DroneRotation;
	player_move->type = CS_PLAYER_MOVE;
	player_move->size = sizeof(cs_packet_player_move);
	send_wsabuf.len = sizeof(cs_packet_player_move);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	

}

void ULOTGameInstance::SendFire(FVector Location, FRotator Rotation, float Power, int Type)
{

	cs_packet_tank_shot *tank_shot = reinterpret_cast<cs_packet_tank_shot *>(send_buffer);

	tank_shot->type = CS_TANK_SHOT;
	tank_shot->size = sizeof(cs_packet_tank_shot);
	tank_shot->location = Location;
	tank_shot->rotation = Rotation;
	tank_shot->power = Power;
	tank_shot->projectile = Type;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("탄 타입 %d"), Type));
	send_wsabuf.len = sizeof(cs_packet_tank_shot);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

}

void ULOTGameInstance::SendFinishLoad()
{
	cs_packet_finish_load *finish_load = reinterpret_cast<cs_packet_finish_load *>(send_buffer);
	finish_load->type = CS_FINISH_LOAD;
	finish_load->size = sizeof(cs_packet_finish_load);
	send_wsabuf.len = sizeof(cs_packet_finish_load);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendTankHit(float Damage,int Type)
{

	cs_packet_tank_hit *tank_hit = reinterpret_cast<cs_packet_tank_hit *>(send_buffer);
	tank_hit->type = CS_TANK_HIT;
	tank_hit->size = sizeof(cs_packet_tank_hit);
	tank_hit->damage = Damage;
	tank_hit->projectile = Type;
	send_wsabuf.len = sizeof(cs_packet_tank_hit);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendExplosion()
{
	cs_packet_tank_explosion *tank_explosion = reinterpret_cast<cs_packet_tank_explosion *>(send_buffer);
	tank_explosion->type = CS_TANK_EXPLOSION;
	tank_explosion->size = sizeof(cs_packet_tank_explosion);
	send_wsabuf.len = sizeof(cs_packet_tank_explosion);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::ReturnMain()
{
	cs_packet_return_main *return_main = reinterpret_cast<cs_packet_return_main *>(send_buffer);
	return_main->type = CS_RETURN_MAIN;
	return_main->size = sizeof(cs_packet_return_main);
	send_wsabuf.len = sizeof(cs_packet_return_main);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendTargeting(bool bTarget)
{
	cs_packet_drone_targeting *drone_targeting = reinterpret_cast< cs_packet_drone_targeting *>(send_buffer);
	drone_targeting->type = CS_DRONE_TARGETING;
	drone_targeting->size = sizeof(cs_packet_drone_targeting);
	drone_targeting->isTargeting = bTarget;
	send_wsabuf.len = sizeof(cs_packet_drone_targeting);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendActivateHoming()
{
	cs_packet_activate_homing *activate_homing = reinterpret_cast< cs_packet_activate_homing*>(send_buffer);
	activate_homing->type = CS_ACTIVATE_HOMING;
	activate_homing->size = sizeof(cs_packet_activate_homing);
	send_wsabuf.len = sizeof(cs_packet_activate_homing);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendActivateBeam()
{
	cs_packet_use_beam *use_beam = reinterpret_cast< cs_packet_use_beam*>(send_buffer);
	use_beam->type = CS_BEAM_ON;
	use_beam->size = sizeof(cs_packet_use_beam);
	send_wsabuf.len = sizeof(cs_packet_use_beam);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendDeactivateBeam()
{
	cs_packet_use_beam *use_beam = reinterpret_cast< cs_packet_use_beam*>(send_buffer);
	use_beam->type = CS_BEAM_OFF;
	use_beam->size = sizeof(cs_packet_use_beam);
	send_wsabuf.len = sizeof(cs_packet_use_beam);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendPlaneLocation(FVector P_Location, FRotator P_Rotation)
{
	cs_packet_plane_move *p_move = reinterpret_cast<cs_packet_plane_move*>(send_buffer); 
	p_move->location = P_Location;
	p_move->rotator = P_Rotation;
	p_move->type = CS_PLANE_MOVE;
	p_move->size = sizeof(cs_packet_plane_move);
	send_wsabuf.len = sizeof(cs_packet_plane_move);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString::Printf(TEXT("비행기 좌표 %f %f %f"), P_Location.X, P_Location.Y, P_Location.Z));
}

void ULOTGameInstance::SendDeletePlane() {
	cs_packet_delete_plane *plane = reinterpret_cast<cs_packet_delete_plane*>(send_buffer);
	bIsPlaneAlive = false;
	plane->type = CS_DELETE_PLANE;
	plane->size = sizeof(cs_packet_delete_plane);
	send_wsabuf.len = sizeof(cs_packet_delete_plane);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Yellow, FString::Printf(TEXT("비행기 삭제 메시지 보냄")));
}

void ULOTGameInstance::SendSpawnItem() {
	cs_packet_spawn_item *item = reinterpret_cast<cs_packet_spawn_item*>(send_buffer);

	ItemsInfo[itemIndex].bIsAlive = true;
	item->type = CS_SPAWN_ITEM;
	item->size = sizeof(cs_packet_spawn_item); 
	send_wsabuf.len = sizeof(cs_packet_spawn_item);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::SendItemLocation(int BoxIndex, FVector I_Location, FRotator I_Rotation, float p_Size)
{
	cs_packet_item_move *i_move = reinterpret_cast<cs_packet_item_move*>(send_buffer);
	ItemsInfo[BoxIndex].item_Location = I_Location;
	ItemsInfo[BoxIndex].item_Rotation = I_Rotation;
	i_move->location = I_Location;
	i_move->rotator = I_Rotation;
	i_move->index = BoxIndex;
	i_move->parachuteSize = p_Size;
	i_move->type = CS_ITEM_MOVE;
	i_move->size = sizeof(cs_packet_item_move);
	send_wsabuf.len = sizeof(cs_packet_item_move);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Blue, FString::Printf(TEXT("아이템 좌표 %f %f %f"), I_Location.X, I_Location.Y, I_Location.Z));
}

void ULOTGameInstance::SendEatItem(int itemNum)
{
	cs_packet_eat_item *item = reinterpret_cast<cs_packet_eat_item*>(send_buffer);

	item->itemNum = itemNum;
	E_itemNum = itemNum;
	ItemsInfo[E_itemNum].bIsAlive = false;
	item->type = CS_EAT_ITEM;
	item->size = sizeof(cs_packet_eat_item);
	send_wsabuf.len = sizeof(cs_packet_eat_item);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL); 
	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString::Printf(TEXT("%d번 아이템을 먹었다고 보냄"), itemNum));
}

void ULOTGameInstance::SendAdjustItem(int itemNum) 
{
	cs_packet_adjust_item *item = reinterpret_cast<cs_packet_adjust_item*>(send_buffer);

	if (itemNum == RESTOREHP) {
		item->type = CS_RESTORE_HP;
	}
	else if (itemNum == RESTORESHIELD) {
		item->type = CS_RESTORE_SHIELD;
	}
	item->size = sizeof(cs_packet_adjust_item);
	send_wsabuf.len = sizeof(cs_packet_adjust_item);
	DWORD iobyte;
	WSASend(sock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void ULOTGameInstance::Disconnect()
{
	if (bIsConnected) {
		closesocket(sock);
		event_thread->detach();
		delete(event_thread);
		WSACleanup();
		bIsConnected = false;
	}
}



