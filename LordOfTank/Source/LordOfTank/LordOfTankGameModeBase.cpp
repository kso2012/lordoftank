// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "PlayerInfo.h"
#include "LordOfTankGameModeBase.h"

#define PawnTank 1
#define PawnDrone 2
#define AIMove 1
#define AIShot 2



PlayerInfo Info = PlayerInfo::PlayerInfo();

ALordOfTankGameModeBase::ALordOfTankGameModeBase()
{
	DroneSpawningHeight = 10000.f;
	DefaultPawnClass = NULL;
	//DefaultPawnClass = ALOTPlayer::StaticClass();
	PlayerCount = 1;
	PlayerTurn = 1;
	PrimaryActorTick.bCanEverTick = true;
}

void ALordOfTankGameModeBase::StartPlay()
{
	Super::StartPlay();

	InitPlayer();

	InitAI();
	
}

void ALordOfTankGameModeBase::InitPlayer() {
	Control = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AActor* const PlayerStart1 = Cast<AActor>(FindPlayerStart(Control, "1"));
	UWorld* const World = GetWorld();

	MyPlayer.Tank = World->SpawnActor<ALOTPlayer>(ALOTPlayer::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
	MyPlayer.Drone = World->SpawnActor<ALOTDrone>(ALOTDrone::StaticClass(), PlayerStart1->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart1->GetActorRotation());
	MyPlayer.PlayerNum = 1;
	MyPlayer.Tank->SetTurn(true);
	MyPlayer.ControlledPawn = PawnTank;
	MyPlayer.Tank->SetisNotAI(true);

	Control->Possess(MyPlayer.Tank);
}

void ALordOfTankGameModeBase::InitAI() {
	
	AI = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 1));
	AActor* const PlayerStart2 = Cast<AActor>(FindPlayerStart(AI, "2"));
	UWorld* const World = GetWorld();


	EnemyPlayer.Tank = World->SpawnActor<ALOTPlayer>(ALOTPlayer::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
	EnemyPlayer.Drone = World->SpawnActor<ALOTDrone>(ALOTDrone::StaticClass(), PlayerStart2->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart2->GetActorRotation());
	EnemyPlayer.PlayerNum = 2;
	EnemyPlayer.Tank->SetTurn(false);
	EnemyPlayer.ControlledPawn = PawnTank;
	EnemyPlayer.Tank->SetisNotAI(false);

	//AI->Possess(EnemyPlayer.Tank);
}


/*
void ALordOfTankGameModeBase::SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum) {
	Info.InsertPawn(T, D, PlayerNum);
	ALordOfTankGameModeBase::IncreasePlayerCount();
}

void ALordOfTankGameModeBase::IncreasePlayerCount() {
	ALordOfTankGameModeBase::PlayerCount ++;
}
*/

void ALordOfTankGameModeBase::ChangePawn() {
	if (PlayerTurn == 1) {
		if (MyPlayer.ControlledPawn == PawnTank && !MyPlayer.Tank->PossessTank && !MyPlayer.Drone->PossessDrone) {
			MyPlayer.ControlledPawn = PawnDrone;
			Control->UnPossess();
			Control->Possess(MyPlayer.Drone);
			MyPlayer.Drone->PossessDrone = true;
		}
		else if(MyPlayer.ControlledPawn == PawnDrone && !MyPlayer.Tank->PossessTank && !MyPlayer.Drone->PossessDrone) {
			MyPlayer.ControlledPawn = PawnTank;
			Control->UnPossess();
			Control->Possess(MyPlayer.Tank);
			MyPlayer.Tank->PossessTank = true;
		}
	}
	else {
		if (EnemyPlayer.ControlledPawn == PawnTank && !EnemyPlayer.Tank->PossessTank && !EnemyPlayer.Drone->PossessDrone) {
			EnemyPlayer.ControlledPawn = PawnDrone;
			Control->UnPossess();
			Control->Possess(EnemyPlayer.Drone);
			EnemyPlayer.Drone->PossessDrone = true;
		}
		else if (EnemyPlayer.ControlledPawn == PawnDrone && !EnemyPlayer.Tank->PossessTank && !EnemyPlayer.Drone->PossessDrone) {
			EnemyPlayer.ControlledPawn = PawnTank;
			Control->UnPossess();
			Control->Possess(EnemyPlayer.Tank);
			EnemyPlayer.Tank->PossessTank = true;
		}
	}
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), PlayerTurn));
}


//float ALordOfTankGameModeBase::ReturnProjectileVelocity() {
//	//ALordOfTankGameModeBase *mode = this;
//	ALOTPlayer *Tank = Info.ReturnData(PlayerTurn).ReturnTank();
//	return (*Tank).ReturnVelocity();
//}

void ALordOfTankGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerTurn == 1) {
		if (!MyPlayer.Tank->GetTurn()) {
			PlayerTurn = 2;
			EnemyPlayer.Tank->SetTurn(true);
		}
	}
	else if (PlayerTurn == 2) {
		if (!EnemyPlayer.Tank->GetTurn()) {
			PlayerTurn = 1;
			MyPlayer.Tank->SetTurn(true);
		}
		EnemyPlayer.Tank->Think();
	}
	ChangePawn();
}

