// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Pawn/LOTPlayer.h"
#include "Pawn/LOTDrone.h"
#include "TrainingMode.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
 struct FTrainingInfomation
{
GENERATED_USTRUCT_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
ALOTPlayer* Tank;
ALOTDrone* Drone;
AActor* TargetActor;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
float AP;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
float HP;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
float Shield;
};


UCLASS()
class LORDOFTANK_API ATrainingMode : public AGameModeBase
{
	GENERATED_BODY()	
	ATrainingMode();

public:
	//class ALOTPlayer* MyPlayer;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaTime) override;


	float DroneSpawningHeight;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FTrainingInfomation MyPlayer;

	FTrainingInfomation ShootPlayer;
	FTrainingInfomation EndPlayer;


private:


	APlayerController* Control;

	void InitPlayer();


	float MaxHealth;

	float MaxAP;

	float MaxShield;

	float MinHeath;

	float MinAP;

	float MinShield;

	void SetInfo();
	
	
};
