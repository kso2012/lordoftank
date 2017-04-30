// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

/**
 * 
 */





UCLASS()
class LORDOFTANK_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AStartGameMode();
	virtual void StartPlay() override;
	UFUNCTION(BlueprintNativeEvent, Category = "NetFuntion")
		void StartMultiGame();
		void StartMultiGame_Implementation();
		
	



};
