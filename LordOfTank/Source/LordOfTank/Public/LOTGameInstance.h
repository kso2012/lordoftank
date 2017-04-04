// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "LOTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API ULOTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	ULOTGameInstance();


	UFUNCTION(BlueprintCallable, Category = "MenuClick")
		void ClickMultiBT();
};
