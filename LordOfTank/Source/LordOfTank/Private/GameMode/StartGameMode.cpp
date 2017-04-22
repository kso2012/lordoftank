// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"
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

//void EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("场!!")));
//	
//	//ULOTGameInstance* const Test = (AActor::GetGameInstance());
//	//ULOTGameInstance* const Test = Cast<ULOTGameInstance>(UGameplayStatics::GetGameInstance());
//	if (Test->bIsConnected)
//	{
//		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("家南力芭!!")));
//		closesocket(Test->sock);
//		Test->event_thread->detach();
//		delete(Test->event_thread);
//		WSACleanup();
//	}
//}