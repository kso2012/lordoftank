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
	bIsInRange = false;
	DroneDirection = FVector(0, 0, 0);
	DroneRotateDirection = None;
	bIsRightDirection = false;
	bIsTurretRightDirection = false;

	MaxHealth = 100.f;

	MaxAP = 2500.f;

	MaxShield = 100.f;

	MinHeath = 0.f;

	MinAP = 0.f;

	MinShield = 0.f;

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

	MyPlayer.Drone->OffViewBox();
	Control->Possess(MyPlayer.Tank);
	MyPlayer.AP = MaxAP;

	MyPlayer.HP = MaxHealth;

	MyPlayer.Shield = MaxShield;
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
	EnemyPlayer.Tank->Player = MyPlayer.Tank;

	EnemyPlayer.Drone->OnViewBox();
	IsEnemyFound = false;
	EnemyPlayer.AP = MaxAP;

	EnemyPlayer.HP = MaxHealth;

	EnemyPlayer.Shield = MaxShield;

	//AI->Possess(EnemyPlayer.Tank);
}


void ALordOfTankGameModeBase::ChangePawn() {
	if (PlayerTurn == 1) {
		if (MyPlayer.ControlledPawn == PawnTank && !MyPlayer.Tank->PossessTank && !MyPlayer.Drone->PossessDrone) {
			MyPlayer.ControlledPawn = PawnDrone;	
			MyPlayer.Drone->PossessDrone = true;
			SyncTankAP();
		}
		else if(MyPlayer.ControlledPawn == PawnDrone && !MyPlayer.Tank->PossessTank && !MyPlayer.Drone->PossessDrone) {
			MyPlayer.ControlledPawn = PawnTank;
			MyPlayer.Tank->PossessTank = true;
			SyncDroneAP();
		}
	}
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), PlayerTurn));
}


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
	}
	else if (PlayerTurn == 2) {
		if (!EnemyPlayer.Tank->GetTurn()) {
			PlayerTurn = 1;
			MyPlayer.Tank->SetTurn(true);
			// 플레이어에게 턴이 넘어올 때 조작을 가능하게 함
			MyPlayer.Tank->EnableInput(Control);
		}
	}
	if (MyPlayer.ControlledPawn == PawnTank)
		SyncTankAP();
	else
		SyncDroneAP();
	ChangePawn();
	Think();
}


void ALordOfTankGameModeBase::Think() {
	// 플레이어나 AI가 포를 발사하고 있을 땐 탱크와 드론의 ViewBox를 사용하지 않음
	// 남겨둘 경우 포탄이 어딘가에 충돌된 것으로 판단하여 바로 터지게 됌
	if (MyPlayer.Tank->GetbIsShoot() || EnemyPlayer.Tank->GetbIsShoot() || MyPlayer.Drone->bDetectMode) {
		EnemyPlayer.Drone->OffViewBox();
		MyPlayer.Drone->OffViewBox();
	}
	else {
		IsLookEnemyTank();
		if (PlayerTurn == 2 && !bIsInRange) {
			if (!bIsRightDirection) {
				SetDroneDirection(); 
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Start Drone Rotate");
				if (DroneRotateDirection != None){
					FVector DVector = UKismetMathLibrary::VLerp(EnemyPlayer.Drone->ReturnForwardVector(), DroneDirection, 0.01f);
					EnemyPlayer.Drone->RotateDrone(DVector.Rotation());

					if (((DroneDirection.X - 0.1f) < EnemyPlayer.Drone->ReturnForwardVector().X && EnemyPlayer.Drone->ReturnForwardVector().X  < (DroneDirection.X + 0.1f))
						 && ((DroneDirection.Y - 0.1f) < EnemyPlayer.Drone->ReturnForwardVector().Y && EnemyPlayer.Drone->ReturnForwardVector().Y < (DroneDirection.Y + 0.1f))) {

						DVector = UKismetMathLibrary::VLerp(EnemyPlayer.Drone->ReturnForwardVector(), DroneDirection, 1.f);
						EnemyPlayer.Drone->RotateDrone(DVector.Rotation());

						bIsRightDirection = true;
						DroneRotateDirection = None;
						DroneDirection = FVector(0, 0, 0);

						//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Drone Rotation Done");
					}
				}
			}
		}
		if(PlayerTurn == 2 && bIsRightDirection && !bIsInRange) {
			EnemyPlayer.Drone->CommandMoveForward(1.f);
			FVector tempLocation = EnemyPlayer.Drone->GetActorLocation();
			tempLocation.Z = MyPlayer.Tank->ReturnMeshLocation().Z;
			float Drone_Tank_Distance = (MyPlayer.Tank->GetActorLocation() - tempLocation).Size();

			if (Drone_Tank_Distance >= 25000.f && Drone_Tank_Distance <= 35000.f) {
				bIsInRange = true;
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "in Range");
			}
			else if (Drone_Tank_Distance < 25000.f) {
				EnemyPlayer.Drone->CommandMoveForward(-2.f);
				SyncDroneAP();
			}

		}
		else if(PlayerTurn == 2 && EnemyPlayer.Drone->ReturnDroneSpeed() != 0.f && bIsInRange) {
			if (EnemyPlayer.Drone->ReturnDroneSpeed() < -50.f) {
				EnemyPlayer.Drone->CommandMoveForward(10.f);
				SyncDroneAP();
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%f FSpeed"), EnemyPlayer.Drone->ReturnDroneSpeed()));
			}
			else if (EnemyPlayer.Drone->ReturnDroneSpeed() < 50.f) {
				EnemyPlayer.Drone->SetDroneSpeed();
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Stop");
			}
			else {
				EnemyPlayer.Drone->CommandMoveForward(-10.f);
				SyncDroneAP();
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%f BSpeed"), EnemyPlayer.Drone->ReturnDroneSpeed()));
			}
		}


		if (PlayerTurn == 2 && bIsInRange && EnemyPlayer.Drone->ReturnDroneSpeed() == 0.f) {
			TraceEnemyLocation();
			// AI 포신이 플레이어 탱크를 제대로 조준하지 않았을 경우 포신을 회전
			if (EnemyPlayer.Tank->TurretAim != CorrectAim) {
				CalcTurretRotator();
				FVector DVector = UKismetMathLibrary::VLerp(EnemyPlayer.Tank->ReturnTurretForwardVector(), TurretDirection, 0.01f);
				EnemyPlayer.Tank->RotateTurret(DVector.Rotation());
				if (((TurretDirection.X - 0.1f) < EnemyPlayer.Tank->ReturnTurretForwardVector().X && EnemyPlayer.Tank->ReturnTurretForwardVector().X  < (TurretDirection.X + 0.1f))
					&& ((TurretDirection.Y - 0.1f) < EnemyPlayer.Tank->ReturnTurretForwardVector().Y && EnemyPlayer.Tank->ReturnTurretForwardVector().Y < (TurretDirection.Y + 0.1f))) {
					
					DVector = UKismetMathLibrary::VLerp(EnemyPlayer.Tank->ReturnTurretForwardVector(), TurretDirection, 1.f);
					EnemyPlayer.Tank->RotateTurret(TurretDirection.Rotation());
					
					
					bIsTurretRightDirection = true;
					TurretRotateDirection = None;
					TurretDirection = FVector(0, 0, 0);

					EnemyPlayer.Tank->TurretAim = CorrectAim;

					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Aim Done");
				}
			}
			// AI 포신이 제대로 조준이 되었을 경우 파워 조절 후 발사
			else if(EnemyPlayer.Tank->TurretAim == CorrectAim && !EnemyPlayer.Tank->bIsWaiting) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SetAim()");
				if (distance == 0.f)
					SetPower();
				//EnemyPlayer.Tank->CommandShoot(distance);
				EnemyPlayer.Tank->SetAim(distance);
				if (EnemyPlayer.Tank->RightShot) {

					bIsTurretRightDirection = false;
					EnemyPlayer.Tank->TurretAim = None;
					distance = 0.f;
				}
			}
		}

	}
	// 플레이어나 AI가 포를 발사하고 있지 않은 경우에는 ViewBox를 사용하는 상태로 둠
	if (!(MyPlayer.Tank->GetbIsShoot() && EnemyPlayer.Tank->GetbIsShoot() && MyPlayer.Drone->bDetectMode)) {
		EnemyPlayer.Drone->OnViewBox();
	}
}


void ALordOfTankGameModeBase::IsLookEnemyTank() {
	if (EnemyPlayer.Drone->DecideCollisionState == Find) {
		if (EnemyPlayer.Drone->CollisionActor == MyPlayer.Tank) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Find Enemy");
			IsEnemyFound = true;
			//DroneDirection = MyPlayer.Tank->GetActorLocation() - EnemyPlayer.Drone->GetActorLocation();
		}
		EnemyPlayer.Drone->DecideCollisionState = None;
	}
	else if (EnemyPlayer.Drone->DecideCollisionState == Lost) {
		if (EnemyPlayer.Drone->CollisionActor == MyPlayer.Tank) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Lost Enemy");
			IsEnemyFound = false;
			bIsRightDirection = false;
			bIsInRange = false;
		}
		EnemyPlayer.Drone->DecideCollisionState = None;
	}
}

void ALordOfTankGameModeBase::TraceEnemyLocation() {
	EnemyPlayer.Tank->SetEnemyLocation(MyPlayer.Tank->ReturnMeshLocation());
}


void ALordOfTankGameModeBase::CalcTurretRotator() {
	FVector TurretLookVector = EnemyPlayer.Tank->ReturnTurretForwardVector();
	FVector TankLocation = EnemyPlayer.Tank->ReturnMeshLocation();

	TurretDirection = MyPlayer.Tank->ReturnMeshLocation() - TankLocation;
	TurretDirection.Z = 0.f;
	TurretDirection.Normalize();

	FVector CrossVector = UKismetMathLibrary::Cross_VectorVector(TurretDirection, TurretLookVector);

	if (UKismetMathLibrary::Dot_VectorVector(CrossVector, FVector(0,0,1)) > 0)
		TurretRotateDirection = Right;
	else
		TurretRotateDirection = Left;
}

void ALordOfTankGameModeBase::SetPower() {
	distance = EnemyPlayer.Tank->GetDistanceTo(MyPlayer.Tank);
}

void ALordOfTankGameModeBase::SetDroneDirection() {
	FVector DroneLocation = EnemyPlayer.Drone->ReturnMeshLocation();
	FVector DroneForwardVector = EnemyPlayer.Drone->ReturnForwardVector();
	if (DroneRotateDirection == None && !bIsRightDirection) {

		
		DroneDirection = MyPlayer.Tank->ReturnMeshLocation() - DroneLocation;
		

		DroneDirection.Z = 0.f;
		DroneDirection.Normalize();
		FVector CrossVector = UKismetMathLibrary::Cross_VectorVector(DroneDirection, DroneForwardVector);
		if (UKismetMathLibrary::Dot_VectorVector(CrossVector, FVector(0, 0, 1)) > 0)
			DroneRotateDirection = Right;
		else
			DroneRotateDirection = Left;


		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Set Drone Direction()");

	}
}

void ALordOfTankGameModeBase::SyncDroneAP() {
	if (MyPlayer.Tank->GetTurn()) {
		MyPlayer.AP = MyPlayer.Drone->ReturnAP();
		if (MyPlayer.AP < 0)
			MyPlayer.AP = 0.f;
		MyPlayer.Tank->SetAP(MyPlayer.AP);
		MyPlayer.Drone->SetAP(MyPlayer.AP);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("MYAP : %f"), MyPlayer.AP));
	}
	else {
		EnemyPlayer.AP = EnemyPlayer.Drone->ReturnAP();
		if (EnemyPlayer.AP < 0)
			EnemyPlayer.AP = 0.f;
		EnemyPlayer.Tank->SetAP(EnemyPlayer.AP);
		EnemyPlayer.Drone->SetAP(EnemyPlayer.AP);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("EnemyAP : %f"), EnemyPlayer.AP));
	}
	
}

void ALordOfTankGameModeBase::SyncTankAP() {
	if (MyPlayer.Tank->GetTurn()) {
		MyPlayer.AP = MyPlayer.Tank->ReturnAP();
		if (MyPlayer.AP < 0)
			MyPlayer.AP = 0.f;
		MyPlayer.Tank->SetAP(MyPlayer.AP);
		MyPlayer.Drone->SetAP(MyPlayer.AP); 
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("MYAP : %f"), MyPlayer.AP));
	}
	else {
		EnemyPlayer.AP = EnemyPlayer.Tank->ReturnAP();
		if (EnemyPlayer.AP < 0)
			EnemyPlayer.AP = 0.f;
		EnemyPlayer.Tank->SetAP(EnemyPlayer.AP);
		EnemyPlayer.Drone->SetAP(EnemyPlayer.AP);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("EnemyAP : %f"), EnemyPlayer.AP));
	}
}