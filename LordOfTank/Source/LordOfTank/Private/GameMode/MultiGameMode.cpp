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

	bIsEndGame = false;

}

void AMultiGameMode::InitPlayer()
{
	MyPlayer.AP = MaxAP;

	MyPlayer.HP = MaxHealth;

	MyPlayer.Shield = MaxShield;

	MyPlayer.Dead = false;

	EnemyPlayer.AP = MaxAP;

	EnemyPlayer.HP = MaxHealth;

	EnemyPlayer.Shield = MaxShield;

	EnemyPlayer.Dead = false;

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
		
		MyPlayer.PlayerNum = 1;
		EnemyPlayer.PlayerNum = 2;
	
	}

	else if (TestInstance->PlayerNum == 2)
	{
		EnemyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart1->GetActorLocation(), PlayerStart1->GetActorRotation());
		EnemyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart1->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart1->GetActorRotation());

		MyPlayer.Tank = World->SpawnActor<ALOTMultiPlayer>(ALOTMultiPlayer::StaticClass(), PlayerStart2->GetActorLocation(), PlayerStart2->GetActorRotation());
		MyPlayer.Drone = World->SpawnActor<ALOTMultiDrone>(ALOTMultiDrone::StaticClass(), PlayerStart2->GetActorLocation() + FVector(0.f, 0.f, DroneSpawningHeight), PlayerStart2->GetActorRotation());
		
		MyPlayer.PlayerNum = 2;
		EnemyPlayer.PlayerNum = 1;

	}

	EnemyPlayer.Tank->SetUI(false);
	EnemyPlayer.Drone->SetUI(false);
	MyPlayer.Drone->SetUI(false);
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

	if (!bIsEndGame) {
		EnemyTargeting();
		TurnChange();
		ApplyMovement();
		EnemyActivateHoming();
		EnemyFire();
		ApplyDamage();
		SetMoveable();
		EndGame();
		SendLocation();

	}
	
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
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 발사 타입은 %d"), MyInstance->EnemyShotProjectileType));
			const FVector InitialVelocity = UKismetMathLibrary::TransformDirection(UKismetMathLibrary::MakeTransform(MyInstance->EnemyShotLocation,
				FRotator(0.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f)), FVector(MyInstance->EnemyShotPower, 0.f, 0.f));
			if (MyInstance->EnemyShotProjectileType == PROJECTILE_COMMON) {
				TempActor = World->SpawnActor<ACommonProjectile>(ACommonProjectile::StaticClass(), MyInstance->EnemyShotLocation, MyInstance->EnemyShotRotation);
				TempActor->SetInitialVelocity(InitialVelocity);
				TempActor->ParentTank = EnemyPlayer.Tank;
				TempActor->SetEnemyFire(true);
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 보통탄 발사")));
			}
			else if (MyInstance->EnemyShotProjectileType == PROJECTILE_ARMORPIERCING)
			{
				TempActor = World->SpawnActor<AArmorPiercingProjectile>(AArmorPiercingProjectile::StaticClass(), MyInstance->EnemyShotLocation, MyInstance->EnemyShotRotation);
				TempActor->SetInitialVelocity(InitialVelocity);
				TempActor->ParentTank = EnemyPlayer.Tank;
				TempActor->SetEnemyFire(true);
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 관통탄 발사")));
			}
			else if (MyInstance->EnemyShotProjectileType == PROJECTILE_HOMING)
			{
				TempActor = World->SpawnActor<AHomingProjectile>(AHomingProjectile::StaticClass(), MyInstance->EnemyShotLocation, MyInstance->EnemyShotRotation);
				TempActor->SetInitialVelocity(InitialVelocity);
				TempActor->ParentTank = EnemyPlayer.Tank;
				TempActor->SetEnemyFire(true);
				if(MyInstance->bIsTarget)
					TempActor->SetHomingTarget(MyPlayer.Tank, MyInstance->EnemyShotPower * 5);
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 유도탄 발사")));

			}
			
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/TankFire_Cue.TankFire_Cue")),
				MyInstance->EnemyShotLocation, MyInstance->EnemyShotRotation);

			UGameplayStatics::PlayWorldCameraShake(GetWorld(), UTankCameraShake::StaticClass(), MyInstance->EnemyShotLocation, 0.f, 500.f, false);
			MyInstance->bEnemyIsShot = false;
		}
	}
}

void AMultiGameMode::ApplyDamage()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if(MyInstance->bIsHurt)
	{
		//1p가 다쳤다면
		if (MyInstance->HitPlayerNum == 1)
		{

			
			//내가 다쳤다면
			if (MyInstance->PlayerNum == 1)
			{
				MyPlayer.Shield = MyInstance->Shield;
				MyPlayer.HP = MyInstance->HP;
				//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("내가 다침 hp = %f,shield = %f!"), MyPlayer.HP, MyPlayer.Shield));

				if (MyPlayer.HP <= 0.f)
				{
					MyPlayer.HP = 0.f;
					MyPlayer.Dead = true;
					MyPlayer.Tank->SetDead();
				}
			}
			//적이 다쳤다면
			if (MyInstance->PlayerNum == 2)
			{
				EnemyPlayer.Shield = MyInstance->Shield;
				EnemyPlayer.HP = MyInstance->HP;
				//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 다침 hp = %f,shield = %f!"), EnemyPlayer.HP, EnemyPlayer.Shield));

				if (EnemyPlayer.HP <= 0.f)
				{
					EnemyPlayer.HP = 0.f;
					EnemyPlayer.Dead = true;
					EnemyPlayer.Tank->SetDead();
				}
			}


			
		
		}
		//2p가 다쳤다면
		else if (MyInstance->HitPlayerNum == 2)
		{
			//내가 다쳤다면
			if (MyInstance->PlayerNum == 2)
			{
				MyPlayer.Shield = MyInstance->Shield;
				MyPlayer.HP = MyInstance->HP;
				//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("내가 다침 hp = %f,shield = %f!"), MyPlayer.HP, MyPlayer.Shield));

				if (MyPlayer.HP <= 0.f)
				{
					MyPlayer.HP = 0.f;
					MyPlayer.Dead = true;
					MyPlayer.Tank->SetDead();
				}
			}
			//적이 다쳤다면
			if (MyInstance->PlayerNum == 1)
			{
				EnemyPlayer.Shield = MyInstance->Shield;
				EnemyPlayer.HP = MyInstance->HP;
				//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 다침 hp = %f,shield = %f!"), EnemyPlayer.HP, EnemyPlayer.Shield));

				if (EnemyPlayer.HP <= 0.f)
				{
					EnemyPlayer.HP = 0.f;
					EnemyPlayer.Dead = true;
					EnemyPlayer.Tank->SetDead();
				}
			}
		}
		MyInstance->bIsHurt = false;
	}
}

void AMultiGameMode::TurnChange()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	//턴이 변경됐다면
	if (MyInstance->bChangeTurnMS)
	{
		//내턴으로 변경됐다면
		if (MyInstance->bIsmyTurn)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("현재 ap = %f"), MyInstance->ChargingAP));
			if (MyPlayer.AP + MyInstance->ChargingAP >= MaxAP)
				MyPlayer.AP = MaxAP;
			else
				MyPlayer.AP += MyInstance->ChargingAP;
		}
		bIsMyTurn = MyInstance->bIsmyTurn;
		MyInstance->bChangeTurnMS = false;
	}
}

void AMultiGameMode::SetMoveable()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());

	if (MyPlayer.AP <= 0 || MyInstance->bIsWaiting)
		MyPlayer.Moveable = false;
	else
		MyPlayer.Moveable = true;

}

void AMultiGameMode::SendLocation()
{

	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if(!bIsEndGame)
	MyInstance->SendLocationInfo(MyPlayer.Tank->GetMesh()->GetPhysicsLinearVelocity(), MyPlayer.Tank->GetMesh()->GetPhysicsAngularVelocity()
		, MyPlayer.Tank->GetMesh()->K2_GetComponentLocation(), MyPlayer.Tank->GetMesh()->K2_GetComponentRotation(), MyPlayer.Drone->GetActorLocation(), MyPlayer.Drone->K2_GetActorRotation());
}

void AMultiGameMode::EndGame()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());

	if (MyInstance->bRecvIsEndGame)
	{
		
		/*FLatentActionInfo LatentActionInfo;
		LatentActionInfo.CallbackTarget = this;
		LatentActionInfo.ExecutionFunction = "OpenLevelCall";
		LatentActionInfo.UUID = 123;
		LatentActionInfo.Linkage = 0;
		UKismetSystemLibrary::Delay(GetWorld(), 10.f, LatentActionInfo);*/

		bIsEndGame = true;
		EndState = MyInstance->EndState;
		MyInstance->bRecvIsEndGame = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("게임끝남!!! = %d"), EndState));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("게임끝남!!! = %d"), EndState));
		OpenLevelCall();
	}
}




void AMultiGameMode::OpenLevelCall_Implementation()
{
	//UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("OpenLevelCall!!!")));
}

void AMultiGameMode::EnemyTargeting()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());

	if (MyInstance->bIsTargetMS)
	{

		if (MyInstance->bIsTarget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 나를 락온")));
			//감지 사운드
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이 나를 락온해제")));//해제 사운드

		MyInstance->bIsTargetMS = false;
	}
}

void AMultiGameMode::EnemyActivateHoming()
{
	ULOTGameInstance* const MyInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if (MyInstance->bEnemyActivateHoming)
	{
		if (AHomingProjectile* const ProjectileType = Cast<AHomingProjectile>(TempActor))
		{
			ProjectileType->ActivateHoming();
			UGameplayStatics::SpawnSound2D(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/HomingWarning.HomingWarning")));
		}
		MyInstance->bEnemyActivateHoming = false;
	}
	
}
