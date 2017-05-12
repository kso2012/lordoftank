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


	//�÷��̾ ���ϱ� ���� ī����
	UPROPERTY(BlueprintReadWrite)
		int PlayerCount;

	//�÷��̾� ��
	UPROPERTY(BlueprintReadWrite)
		int PlayerTurn;

	/*
	//�÷��̾� �����͸� �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
		void SavePlayerInfo(ALOTPlayer *T, ALOTDrone *D, int PlayerNum);

	//�÷��̾ �����Ϳ� ����Ǹ� �÷��̾� ī��Ʈ�� ������Ŵ
	UFUNCTION(BlueprintCallable)
		void IncreasePlayerCount();

	//�߻�� ����ü�� �ӵ��� ��ȯ�ϴ� �Լ�
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

	//�÷��̾�� AI�� ������Ű�� �ʱ�ȭ�ϴ� �Լ�
	void InitPlayer();
	void InitAI();

	//������� Tick���� �÷��̾��� ��ũ�� �������� ���Դ��� �ƴ����� �Ǵ��ϴ� �Լ�
	void IsLookEnemyTank();

	//������� ���� ��ġ�� ã�Ҵٸ� true
	bool IsEnemyFound;

	//������ ��,�� ���ư� ����
	float TurretRotateDirection;

	//�÷��̾�� AI ��ũ�� �Ÿ�
	float distance;

	//AI�� ������ ������ �ִ� �Լ�
	void Think();

	//�÷��̾��� ��ġ�� �����ϴ� �Լ�
	void TraceEnemyLocation();

	//AI�� ������ ��� �������� ȸ�����Ѿ� �ϴ��� ����ϴ� �Լ�, LOTPlayer�� RotateTurret�� ȣ��
	void CalcTurretRotator();

	//�Ÿ��� �Ŀ��� ���ϴ� �Լ�
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
