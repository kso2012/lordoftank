// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"
#include "MultiGameMode.h"




AMultiGameMode::AMultiGameMode()
{
	i = 0;
	DroneSpawningHeight = 10000.f;
	DefaultPawnClass = NULL;
	PrimaryActorTick.bCanEverTick = true;
}

void AMultiGameMode::StartPlay()
{
	Super::StartPlay();


	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AActor* const PlayerStart1 = Cast<AActor>(FindPlayerStart(Test, "1"));
	AActor* const PlayerStart2 = Cast<AActor>(FindPlayerStart(Test, "2"));

	UWorld* const World = GetWorld();

	

	ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if (TestInstance->PlayerNum == 1)
	{
		MyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
		MyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart1->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart1->GetActorRotation());

		EnemyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
		EnemyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart2->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart2->GetActorRotation());
	}

	else if (TestInstance->PlayerNum == 2)
	{
		EnemyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
		EnemyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart1->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart1->GetActorRotation());

		MyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
		MyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart2->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart2->GetActorRotation());
	}


	Test->Possess(MyPlayer.Tank);
	//MyPlayer.Tank->EnableInput(Test);
	//Test->AcknowledgePossession(MyPlayer.Tank);


}

void AMultiGameMode::BeginPlay()
{
	Super::BeginPlay();
	

}



void AMultiGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	ApplyMovement();
	
	//SetWorldTransform();
	//SetWorldTransform(Test->Location, false, nullptr, ETeleportType::TeleportPhysics);
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("X값 %f Y값 %f,z값 %f"), Test->Location.GetLocation().X, Test->Location.GetLocation().Y,Test->Location.GetLocation().Z));
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("속도 %f/,속도 %f/,속도 %f"), MyPlayer.Tank->GetVelocity().X, MyPlayer.Tank->GetVelocity().Y, MyPlayer.Tank->GetVelocity().Z));
	//MyPlayer.Tank->SetActorTransform(Test->Location, false, nullptr, ETeleportType::TeleportPhysics);
	//MyPlayer.Tank->GetMesh()->SetWorldTransform(Test->Location, false, nullptr, ETeleportType::TeleportPhysics);
	//MyPlayer.Tank->GetMesh()->SetAllPhysicsLinearVelocity(Test->Velocity, false);
	//MyPlayer.Tank->SetActorRelativeTransform(Test->Location, true, nullptr, ETeleportType::TeleportPhysics);
}

void AMultiGameMode::ApplyMovement()
{
	ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if (TestInstance->PlayerNum == 1)
	{

		MyPlayer.Tank->GetRootComponent()->SetWorldTransform(TestInstance->Location, false, nullptr, ETeleportType::TeleportPhysics);
		EnemyPlayer.Tank->GetRootComponent()->SetWorldTransform(TestInstance->Location2, false, nullptr, ETeleportType::TeleportPhysics);
	}

	if (TestInstance->PlayerNum == 2)
	{
		MyPlayer.Tank->GetRootComponent()->SetWorldTransform(TestInstance->Location2, false, nullptr, ETeleportType::TeleportPhysics);
		EnemyPlayer.Tank->GetRootComponent()->SetWorldTransform(TestInstance->Location, false, nullptr, ETeleportType::TeleportPhysics);
	}

	if (TestInstance->bIsmyTurn)
	{
		if(TestInstance->PlayerNum == 1)
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("내턴이다!!!(1p)")));
		else
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("내턴이다!!!(2p)")));
		MyPlayer.Tank->bIsTurn = true;
	}
	else
		MyPlayer.Tank->bIsTurn = false;
}

