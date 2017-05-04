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
	float HP;
	float PlayerNum;
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
	

	PlayerInfomation MyPlayer;
	PlayerInfomation EnemyPlayer;

private:
	
	float DroneSpawningHeight;

};
