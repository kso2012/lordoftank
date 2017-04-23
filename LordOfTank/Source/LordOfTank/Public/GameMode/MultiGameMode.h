// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Pawn/LOTPlayer.h"
#include "Pawn/LOTDrone.h"
#include "MultiGameMode.generated.h"

/**
 * 
 */
struct PlayerInfomation
{
	ALOTPlayer* Tank;
	ALOTDrone* Drone;
	AActor* TargetActor;
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

	float DroneSpawningHeight;

private:
	PlayerInfomation MyPlayer;
	PlayerInfomation EnemyPlayer;
	
};
