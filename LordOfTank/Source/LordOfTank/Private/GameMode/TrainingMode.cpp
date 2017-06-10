// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pawn/LOTPlayer.h"
#include "Pawn/LOTDrone.h"
#include "Kismet/GameplayStatics.h"
#include "TrainingMode.h"




ATrainingMode::ATrainingMode()
{
	DroneSpawningHeight = 10000.f;
	DefaultPawnClass = NULL;
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.f;

	MaxAP = 2500.f;

	MaxShield = 100.f;

	MinHeath = 0.f;

	MinAP = 0.f;

	MinShield = 0.f;

}

void ATrainingMode::StartPlay()
{
	Super::StartPlay();

	InitPlayer();

}

void ATrainingMode::InitPlayer() {
	Control = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AActor* const PlayerStart1 = Cast<AActor>(FindPlayerStart(Control, "1"));
	UWorld* const World = GetWorld(); FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	MyPlayer.Tank = World->SpawnActor<ALOTPlayer>(ALOTPlayer::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation(), SpawnInfo);
	MyPlayer.Drone = World->SpawnActor<ALOTDrone>(ALOTDrone::StaticClass(), PlayerStart1->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart1->GetActorRotation(), SpawnInfo);
	MyPlayer.Tank->SetTurn(true);
	MyPlayer.Tank->SetisNotAI(true);

	Control->Possess(MyPlayer.Tank);
	MyPlayer.AP = MaxAP;

	MyPlayer.HP = MaxHealth;

	MyPlayer.Shield = MaxShield;


	AActor* const PlayerStart2 = Cast<AActor>(FindPlayerStart(Control, "2"));
	ShootPlayer.Tank = World->SpawnActor<ALOTPlayer>(ALOTPlayer::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation(), SpawnInfo);
	ShootPlayer.Tank->SetisNotAI(false);
	ShootPlayer.Tank->SetTurn(true);

	AActor* const PlayerStart3 = Cast<AActor>(FindPlayerStart(Control, "3"));
	EndPlayer.Tank = World->SpawnActor<ALOTPlayer>(ALOTPlayer::StaticClass(), PlayerStart3->GetActorLocation(), PlayerStart3->GetActorRotation(), SpawnInfo);
	EndPlayer.Tank->SetisNotAI(false);
}

void ATrainingMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	SetInfo();
	if (!ShootPlayer.Tank->bIsShoot)
		ShootPlayer.Tank->CommandShoot(50000);
	ShootPlayer.Tank->SetTurn(true);

	if (EndPlayer.Tank->HP <= 0)
		UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("ÇÇ= %f, ½Çµå = %f"), EndPlayer.Tank->HP, EndPlayer.Tank->Shield));
}

void ATrainingMode::SetInfo() {
	MyPlayer.HP = MyPlayer.Tank->HP;
	MyPlayer.Shield = MyPlayer.Tank->Shield;
}