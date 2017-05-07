// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"
#include "MultiGameMode.h"




AMultiGameMode::AMultiGameMode()
{
	
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
	TestInstance->SendFinishLoad();
}

void AMultiGameMode::BeginPlay()
{
	Super::BeginPlay();
	

}



void AMultiGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyMovement();


	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	//droneTransform
	MyInstance->SendLocationInfo(MyPlayer.Tank->GetMesh()->GetPhysicsLinearVelocity(), MyPlayer.Tank->GetMesh()->GetPhysicsAngularVelocity()
		, MyPlayer.Tank->GetMesh()->K2_GetComponentLocation(), MyPlayer.Tank->GetMesh()->K2_GetComponentRotation(), MyPlayer.Drone->GetActorLocation(), MyPlayer.Drone->K2_GetActorRotation());

	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("X = %f,Y = %f Z = %f"), MyPlayer.Drone->GetActorLocation().X, MyPlayer.Drone->GetActorLocation().Y, MyPlayer.Drone->GetActorLocation().Z));
}

void AMultiGameMode::ApplyMovement()
{

	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	
	EnemyPlayer.Tank->GetMesh()->SetPhysicsLinearVelocity(MyInstance->EnemyLinearVelocity);
	EnemyPlayer.Tank->GetMesh()->SetPhysicsAngularVelocity(MyInstance->EnemyAngularVelocity);
	EnemyPlayer.Tank->SetActorLocation(MyInstance->EnemyWorldLocation, false, nullptr, ETeleportType::TeleportPhysics);
	EnemyPlayer.Tank->SetActorRotation(MyInstance->EnemyRotation, ETeleportType::TeleportPhysics);
	EnemyPlayer.Drone->SetActorLocation(MyInstance->EnemyDroneLocation, false, nullptr, ETeleportType::TeleportPhysics);
	EnemyPlayer.Drone->SetActorRotation(MyInstance->EnemyDroneRotation, ETeleportType::TeleportPhysics);
	
	

}

