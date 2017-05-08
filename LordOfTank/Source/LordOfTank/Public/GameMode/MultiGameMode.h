// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Pawn/LOTMultiPlayer.h"
#include "Pawn/LOTMultiDrone.h"
#include "MultiGameMode.generated.h"

/**
 * 
 */
struct PlayerInfomation
{
	ALOTMultiPlayer* Tank;
	ALOTMultiDrone* Drone;
	AActor* TargetActor;
	FVector Velocity;
	FVector Angular;
	float AP;
	float Shield;
	float HP;
	float PlayerNum;
	bool Moveable;
};

UCLASS()
class LORDOFTANK_API AMultiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMultiGameMode();
	
	float i;
	
	virtual void StartPlay() override;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	void ApplyMovement();
	
	void ApplyDamage();
	
	void EnemyFire();
	
	void InitPlayer();

	void TurnChange();
	
	PlayerInfomation MyPlayer;
	
	PlayerInfomation EnemyPlayer;

	bool bIsMyTurn;

private:
	
	float DroneSpawningHeight;

	float MaxHealth;

	float MaxAP;

	float MaxShield;

	float MinHeath;

	float MinAP;

	float MinShield;

	

};
