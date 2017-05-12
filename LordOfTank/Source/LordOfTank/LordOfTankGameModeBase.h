// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Pawn/LOTPlayer.h"
#include "Pawn/LOTDrone.h"
#include "PlayerInfo.h"
#include "LordOfTankGameModeBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerInfomations
{
	GENERATED_USTRUCT_BODY()

	ALOTPlayer* Tank;
	ALOTDrone* Drone;
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float AP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float Shield;
	int PlayerNum;
	int ControlledPawn;
};


class FHouse
{
public:
	int32 Size = 10;
};


UCLASS()
class LORDOFTANK_API ALordOfTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALordOfTankGameModeBase();

	//class ALOTPlayer* MyPlayer;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaTime) override;


	//플레이어를 비교하기 위한 카운터
	UPROPERTY(BlueprintReadWrite)
		int PlayerCount;

	//플레이어 턴
	UPROPERTY(BlueprintReadWrite)
		int PlayerTurn;

	/*
	//플레이어 데이터를 저장하는 함수
	UFUNCTION(BlueprintCallable)
		void SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum);

	//플레이어가 데이터에 저장되면 플레이어 카운트를 증가시킴
	UFUNCTION(BlueprintCallable)
		void IncreasePlayerCount();

	//발사된 투사체의 속도를 반환하는 함수
	float ReturnProjectileVelocity();
	*/

	float DroneSpawningHeight;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FPlayerInfomations MyPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FPlayerInfomations EnemyPlayer;


private:


	APlayerController* Control;
	APlayerController* AI;


	float MaxHealth;

	float MaxAP;

	float MaxShield;

	float MinHeath;

	float MinAP;

	float MinShield;

	UFUNCTION(BlueprintCallable)
		void ChangePawn();

	//플레이어와 AI를 생성시키고 초기화하는 함수
	void InitPlayer();
	void InitAI();

	//드론으로 Tick마다 플레이어의 탱크가 범위내에 들어왔는지 아닌지를 판단하는 함수
	void IsLookEnemyTank();

	//드론으로 적의 위치를 찾았다면 true
	bool IsEnemyFound;

	//포신이 좌,우 돌아갈 방향
	float TurretRotateDirection;

	//플레이어와 AI 탱크의 거리
	float distance;

	//AI의 동작을 가지고 있는 함수
	void Think();

	//플레이어의 위치를 추적하는 함수
	void TraceEnemyLocation();

	//AI의 포신을 어느 방향으로 회전시켜야 하는지 계산하는 함수, LOTPlayer의 RotateTurret을 호출
	void CalcTurretRotator();

	//거리로 파워를 정하는 함수
	void SetPower();

	void SetDroneDirection();

	FVector TurretDirection;

	bool bIsTurretRightDirection;



	bool bIsInRange;

	bool bIsRightDirection;

	FVector DroneDirection;

	int DroneRotateDirection;

	void SyncTankAP();

	void SyncDroneAP();

};
