// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Pawn/LOTMultiPlayer.h"
#include "Pawn/LOTMultiDrone.h"
#include "MultiGameMode.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerInfomation
{
	GENERATED_USTRUCT_BODY()
	
	ALOTMultiPlayer* Tank;
	ALOTMultiDrone* Drone;
	AActor* TargetActor;
	FVector Velocity;
	FVector Angular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float AP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float Shield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		float HP;
	
	float PlayerNum;
	bool Moveable;
	bool DroneMoveable;
	bool Dead;
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

	/*UFUNCTION(BlueprintCallable, Category = "Custom")
		void OpenLevelCall();*/

	UFUNCTION(BlueprintNativeEvent, Category = "NetFuntion")
		void OpenLevelCall();
		void OpenLevelCall_Implementation();
	
	
	void ApplyMovement();
	
	void ApplyDamage();
	
	void ApplyEmp();
	
	void EnemyFire();

	void SetMoveable();

	void SendLocation();

	void EndGame();
	
	void InitPlayer();

	void TurnChange();

	void EnemyTargeting();

	void EnemyActivateHoming();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FPlayerInfomation MyPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FPlayerInfomation EnemyPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playing")
		bool bIsEndGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playing")
		int EndState;

	bool bIsMyTurn;

private:
	
	float DroneSpawningHeight;

	float MaxHealth;

	float MaxAP;

	float MaxShield;

	float MinHeath;

	float MinAP;

	float MinShield;

	AProjectile* TempActor;



	

};
