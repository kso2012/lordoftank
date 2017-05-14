// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTPlayer.h"
#include "LOTGameInstance.h"
#include "StartGameMode.h"





AStartGameMode::AStartGameMode()
{
	// set default pawn class to our Blueprinted character 
	//static ConstructorHelpers::FClassFinder<APawn> MenuSelectBP(TEXT("Blueprint'/Game/Blueprints/Main_Blueprints/Menu_Select_Pawn.Menu_Select_Pawn_C'"));
	//if (MenuSelectBP.Class != NULL)
	//{
	//	DefaultPawnClass = MenuSelectBP.Class;
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("dd")));
	//}

}

//void AStartGameMode::test()
//{
//	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Test BT WORK!!")));
//}

void AStartGameMode::StartMultiGame_Implementation()
{
	//UGameplayStatics::OpenLevel(GetWorld(),"NetworkTest");
	//UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "2"), Category = "Game")
		//static void OpenLevel(const UObject* WorldContextObject, FName LevelName, bool bAbsolute = true, FString Options = FString(TEXT("")));

	//UWorld* const World = GetWorld();
	//AActor* LoadingScreen = NewObject<AActor>(this);
	//LoadingScreen = LoadObject<AActor>(nullptr, TEXT("Blueprint'/Game/Blueprints/Main_Blueprints/LoadingScreen.LoadingScreen'"));
	//World->SpawnActor<AActor>(LoadingScreen->GetClass(), FVector(-48255.328125, 31466.636719, 776.058350), FRotator(0.f, 0.f, 0.f));
}
void AStartGameMode::StartPlay()
{
	Super::StartPlay();

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
