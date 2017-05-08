// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Weapon/Projectile.h"
#include "Weapon/CommonProjectile.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/HomingProjectile.h"
#include "Effects/TankCameraShake.h"
#include "MultiGameMode.h"




AMultiGameMode::AMultiGameMode()
{
	
	DroneSpawningHeight = 10000.f;
	
	DefaultPawnClass = NULL;
	
	PrimaryActorTick.bCanEverTick = true;
	

	
	MaxHealth = 100.f;

	MaxAP = 2500.f;

	MaxShield = 100.f;

	MinHeath = 0.f;

	MinAP = 0.f;

	MinShield = 0.f;

	bIsMyTurn = false;

}

void AMultiGameMode::InitPlayer()
{
	MyPlayer.AP = MaxAP;

	MyPlayer.HP = MaxHealth;

	MyPlayer.Shield = MaxShield;

	EnemyPlayer.AP = MaxAP;

	EnemyPlayer.HP = MaxHealth;

	EnemyPlayer.Shield = MaxShield;

	MyPlayer.Moveable = true;
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
	InitPlayer();
	TestInstance->SendFinishLoad();
}

void AMultiGameMode::BeginPlay()
{
	Super::BeginPlay();
	

}



void AMultiGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurnChange();
	ApplyMovement();
	EnemyFire();

	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());

	if (MyPlayer.AP <= 0)
		MyPlayer.Moveable = false;
	else
		MyPlayer.Moveable = true;

	MyInstance->SendLocationInfo(MyPlayer.Tank->GetMesh()->GetPhysicsLinearVelocity(), MyPlayer.Tank->GetMesh()->GetPhysicsAngularVelocity()
		, MyPlayer.Tank->GetMesh()->K2_GetComponentLocation(), MyPlayer.Tank->GetMesh()->K2_GetComponentRotation(), MyPlayer.Drone->GetActorLocation(), MyPlayer.Drone->K2_GetActorRotation());

	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("현재 ap = %f"), MyPlayer.AP));
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

void AMultiGameMode::EnemyFire()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());

	if (MyInstance->bEnemyIsShot)
	{

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FVector InitialVelocity = UKismetMathLibrary::TransformDirection(UKismetMathLibrary::MakeTransform(MyInstance->EnemyShotLocation,
				FRotator(0.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f)), FVector(MyInstance->EnemyShotPower, 0.f, 0.f));


			ACommonProjectile* TempActor = World->SpawnActor<ACommonProjectile>(ACommonProjectile::StaticClass(), MyInstance->EnemyShotLocation, MyInstance->EnemyShotRotation);
			TempActor->SetInitialVelocity(InitialVelocity);
			TempActor->ParentTank = EnemyPlayer.Tank;
			TempActor->SetEnemyFire(true);
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), UTankCameraShake::StaticClass(), MyInstance->EnemyShotLocation, 0.f, 500.f, false);
			MyInstance->bEnemyIsShot = false;
		}
	}
}

void AMultiGameMode::ApplyDamage()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if (MyInstance->bIsHurt)
	{
		//1피가 다쳤다면
		if (MyInstance->HitPlayerNum == 1)
		{
			MyPlayer.Shield = MyInstance->Shield;
			MyPlayer.HP = MyInstance->HP;
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("1피가 다침 hp = %f,shield = %f!"), MyPlayer.HP, MyPlayer.Shield));
		
		}
		//2피가 다쳤다면
		else if (MyInstance->HitPlayerNum == 2)
		{
			EnemyPlayer.Shield = MyInstance->Shield;
			EnemyPlayer.HP = MyInstance->HP;
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("2피가 다침 hp = %f,shield = %f!"), EnemyPlayer.HP, EnemyPlayer.Shield));
		}
		MyInstance->bIsHurt = false;
	}
}

void AMultiGameMode::TurnChange()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	//턴이 변경됐다면
	if (MyInstance->bChangeTurn)
	{
		//내턴으로 변경됐다면
		if (MyInstance->bIsmyTurn)
		{
			
			if (MyPlayer.AP + MyInstance->ChargingAP >= MaxAP)
				MyPlayer.AP = MaxAP;
			else
				MyPlayer.AP += MyInstance->ChargingAP;
		}
		bIsMyTurn = MyInstance->bIsmyTurn;
		MyInstance->bChangeTurn = false;
	}
}
