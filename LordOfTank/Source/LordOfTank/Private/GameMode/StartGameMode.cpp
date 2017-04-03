// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "StartGameMode.h"





AStartGameMode::AStartGameMode()
{
	// set default pawn class to our Blueprinted character 
	static ConstructorHelpers::FClassFinder<APawn> MenuSelectBP(TEXT("Blueprint'/Game/Blueprints/Main_Blueprints/Menu_Select_Pawn.Menu_Select_Pawn_C'"));
	if (MenuSelectBP.Class != NULL)
	{
		DefaultPawnClass = MenuSelectBP.Class;
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("dd")));
	}

}

void AStartGameMode::test()
{
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Test BT WORK!!")));
}