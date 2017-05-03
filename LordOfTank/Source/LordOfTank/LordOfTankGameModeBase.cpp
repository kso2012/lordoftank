// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "PlayerInfo.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "LordOfTankGameModeBase.h"

#define PawnTank 1
#define PawnDrone 2


#define None 0
#define Find 1
#define Lost 2


#define CorrectAim 1
#define InCorrectAim 2

#define Right 1.f
#define Left -1.f



PlayerInfo Info = PlayerInfo::PlayerInfo();

ALordOfTankGameModeBase::ALordOfTankGameModeBase()
{
	DroneSpawningHeight = 10000.f;
	DefaultPawnClass = NULL;
	//DefaultPawnClass = ALOTPlayer::StaticClass();
	PlayerCount = 1;
	PlayerTurn = 1;
	PrimaryActorTick.bCanEverTick = true;
	TurretRotateDirection = None;
	distance = 0.f;
	bIsPreFound = false;
	DroneDirection = FVector(0, 0, 0);
	DroneRotateDirection = None;
	bIsRightDirection = false;
	LostLocation = FVector(0, 0, 0);
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

	MyPlayer.Tank->OffViewBox();
	MyPlayer.Drone->OffViewBox();
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

	EnemyPlayer.Tank->OnViewBox();
	EnemyPlayer.Drone->OnViewBox();
	IsEnemyFound = false;

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
			// AI에게 턴이 넘어갈 때 조작을 금지시킴
			MyPlayer.Tank->DisableInput(Control);
		}
		//IsLookEnemyTank();
	}
	else if (PlayerTurn == 2) {
		if (!EnemyPlayer.Tank->GetTurn()) {
			PlayerTurn = 1;
			MyPlayer.Tank->SetTurn(true);
			// 플레이어에게 턴이 넘어올 때 조작을 가능하게 함
			MyPlayer.Tank->EnableInput(Control);
		}
	}
	ChangePawn();
	Think();
}


void ALordOfTankGameModeBase::Think() {
	// 플레이어나 AI가 포를 발사하고 있을 땐 탱크와 드론의 ViewBox를 사용하지 않음
	// 남겨둘 경우 포탄이 어딘가에 충돌된 것으로 판단하여 바로 터지게 됌
	if (MyPlayer.Tank->GetbIsShoot() || EnemyPlayer.Tank->GetbIsShoot()) {
		EnemyPlayer.Tank->OffViewBox();
		EnemyPlayer.Drone->OffViewBox();
		MyPlayer.Tank->OffViewBox();
		MyPlayer.Drone->OffViewBox();
	}
	else {
		IsLookEnemyTank();
		if ((!IsEnemyFound && PlayerTurn == 2) || (EnemyPlayer.Drone->ReturnDroneSpeed() != 0.f && PlayerTurn == 2)) {
			if (!bIsRightDirection) {
				SetDroneDirection();
				if (DroneRotateDirection != None){
					//EnemyPlayer.Drone->RotateDrone(DroneRotateDirection);
					FVector DVector = UKismetMathLibrary::VLerp(EnemyPlayer.Drone->GetActorForwardVector(), DroneDirection, 0.01f);
					EnemyPlayer.Drone->SetActorRotation(DVector.Rotation());
					
					if (((DroneDirection.X - 0.02f) < EnemyPlayer.Drone->GetActorForwardVector().X && EnemyPlayer.Drone->GetActorForwardVector().X  < (DroneDirection.X + 0.02f)) 
						 && ((DroneDirection.Y - 0.02f) < EnemyPlayer.Drone->GetActorForwardVector().Y && EnemyPlayer.Drone->GetActorForwardVector().Y < (DroneDirection.Y + 0.02f))
						 && ((DroneDirection.Z - 0.02f) < EnemyPlayer.Drone->GetActorForwardVector().Z && EnemyPlayer.Drone->GetActorForwardVector().Z < (DroneDirection.Z + 0.02f))) {
						bIsRightDirection = true;
						DroneRotateDirection = None;
						DroneDirection = FVector(0, 0, 0);

						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Drone Rotate Done");
					}
				}
			}
			else {
				if(!IsEnemyFound)
					EnemyPlayer.Drone->CommandMoveForward(1.f);
				else {
					EnemyPlayer.Drone->CommandMoveForward(-1.f);
					if (EnemyPlayer.Drone->ReturnDroneSpeed() < 10.f)
						EnemyPlayer.Drone->SetDroneSpeed();
				}
			}
		}
		else if (IsEnemyFound && PlayerTurn == 2) {
			TraceEnemyLocation();
			AimTurret();
			// AI 포신이 플레이어 탱크를 제대로 조준하지 않았을 경우 포신을 회전
			if (EnemyPlayer.Tank->TurretAim != CorrectAim) {
				CalcTurretRotator();
				if (TurretRotateDirection != None) {
					EnemyPlayer.Tank->RotateTurret(TurretRotateDirection);
				}
			}
			// AI 포신이 제대로 조준이 되었을 경우 파워 조절 후 발사
			else {
				if (distance == 0.f)
					SetPower();
				EnemyPlayer.Tank->CommandShoot(distance);
			}
		}

	}
	// 플레이어나 AI가 포를 발사하고 있지 않은 경우에는 ViewBox를 사용하는 상태로 둠
	if (!(MyPlayer.Tank->GetbIsShoot() && EnemyPlayer.Tank->GetbIsShoot())) {
		EnemyPlayer.Tank->OnViewBox();
		EnemyPlayer.Drone->OnViewBox();
	}
}


void ALordOfTankGameModeBase::IsLookEnemyTank() {
	if (EnemyPlayer.Drone->DecideCollisionState == Find) {
		if (EnemyPlayer.Drone->CollisionActor == MyPlayer.Tank) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Find Enemy");
			IsEnemyFound = true;
			bIsPreFound = true;
			bIsRightDirection = true;
			LostLocation = FVector(0, 0, 0);
			//DroneDirection = MyPlayer.Tank->GetActorLocation() - EnemyPlayer.Drone->GetActorLocation();
		}
		EnemyPlayer.Drone->DecideCollisionState = None;
	}
	else if (EnemyPlayer.Drone->DecideCollisionState == Lost) {
		if (EnemyPlayer.Drone->CollisionActor == MyPlayer.Tank) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Lost Enemy");
			IsEnemyFound = false;
			LostLocation = MyPlayer.Tank->GetActorLocation();
			bIsRightDirection = false;
		}
		EnemyPlayer.Drone->DecideCollisionState = None;
	}
}

void ALordOfTankGameModeBase::TraceEnemyLocation() {
	EnemyPlayer.Tank->SetEnemyLocation(MyPlayer.Tank->GetActorLocation());
}

void ALordOfTankGameModeBase::AimTurret() {
	if (EnemyPlayer.Tank->CollisionActor == MyPlayer.Tank && EnemyPlayer.Tank->TurretAim == CorrectAim) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Aim Correctly");
		TurretRotateDirection = None;
		distance = 0.f;
	}
	else {
		EnemyPlayer.Tank->TurretAim = None;
	}
}

void ALordOfTankGameModeBase::CalcTurretRotator() {
	FVector TurretLookVector = EnemyPlayer.Tank->ReturnTurretForwardVector();

	FVector CrossVector = UKismetMathLibrary::Cross_VectorVector(EnemyPlayer.Tank->ReturnMeshLocation() - MyPlayer.Tank->ReturnMeshLocation(), TurretLookVector);

	if (TurretRotateDirection == None) {
		if (UKismetMathLibrary::Dot_VectorVector(CrossVector, FVector(0,0,1)) > 0)
			TurretRotateDirection = Right;
		else
			TurretRotateDirection = Left;
	}
}

void ALordOfTankGameModeBase::SetPower() {
	distance = EnemyPlayer.Tank->GetDistanceTo(MyPlayer.Tank);
}

void ALordOfTankGameModeBase::SetDroneDirection() {
	FVector DroneLocation = EnemyPlayer.Drone->GetActorLocation();
	FVector DroneForwardVector = EnemyPlayer.Drone->GetActorForwardVector();
	if (DroneRotateDirection == None && !bIsRightDirection) {

		if (!bIsPreFound) {
			AActor* const PlayerStart = Cast<AActor>(FindPlayerStart(Control, "1"));
			DroneDirection = PlayerStart->GetActorLocation() - DroneLocation;
		}
		else if (bIsPreFound && LostLocation != FVector(0, 0, 0)) {
			DroneDirection = LostLocation - DroneLocation;
		}

		DroneDirection.Z = 0.f;
		DroneDirection.Normalize();
		FVector CrossVector = UKismetMathLibrary::Cross_VectorVector(DroneDirection, DroneForwardVector);
		if (UKismetMathLibrary::Dot_VectorVector(CrossVector, FVector(0, 0, 1)) > 0)
			DroneRotateDirection = Right;
		else
			DroneRotateDirection = Left;

	}
}