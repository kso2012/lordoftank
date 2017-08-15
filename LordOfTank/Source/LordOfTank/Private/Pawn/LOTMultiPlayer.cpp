// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Vehicle/FrontWheel.h"
#include "Vehicle/RearWheel.h"
#include "Weapon/CommonProjectile.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/HomingProjectile.h"
#include "Weapon/Projectile.h"
#include "Weapon/EmpProjectile.h"
#include "Effects/TankCameraShake.h"
#include "LOTGameInstance.h"
#include "GameMode/MultiGameMode.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LOTMultiPlayer.h"




ALOTMultiPlayer::ALOTMultiPlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	//���̷���������Ʈ�� �޽� ����.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySkeletalMesh(TEXT("/Game/LOTAssets/TankAssets/LOTBody.LOTBody"));
	GetMesh()->SetSkeletalMesh(BodySkeletalMesh.Object);
	//���̷���������Ʈ�� �ִϸ��̼� ����.
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/LOTAssets/TankAssets/LOTPlaytankAnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	//GetMesh()->OnComponentHit.AddDynamic(this, &ALOTMultiPlayer::OnHit);
	//GetMesh()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ALOTMultiPlayer::OnOverlapBegin);

	//�ͷ�������Ʈ�� �޽� ����.
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretStaticMesh(TEXT("/Game/LOTAssets/TankAssets/Meshes/LBX1Turret_SM"));
	TurretMesh->SetStaticMesh(TurretStaticMesh.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> TurretMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	//TurretMesh->SetMaterial(0, TurretMaterial.Object);
	//TurretMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	TurretMesh->SetupAttachment(GetMesh(), TEXT("Body_TR"));
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelStaticMesh(TEXT("/Game/LOTAssets/TankAssets/Meshes/LBX1Barrel_SM"));
	BarrelMesh->SetStaticMesh(BarrelStaticMesh.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> BarrelMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	//BarrelMesh->SetMaterial(0, BarrelMaterial.Object);
	//BarrelMesh->AttachToComponent(TurretMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Turret_BR"));
	BarrelMesh->SetupAttachment(TurretMesh, TEXT("Turret_BR"));
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//�ѱ��� ��������Ʈ ����.
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//MuzzleLocation->AttachToComponent(BarrelMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	MuzzleLocation->SetupAttachment(BarrelMesh, TEXT("Muzzle"));



	// ������ �� Ŭ���� ����
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Front_LW");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Front_RW");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = URearWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Rear_LW");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = URearWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Rear_RW");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 0.f, 0.f);




	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	//SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	//SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->TargetArmLength = 600.0f;
	//SpringArm->bEnableCameraRotationLag = true;
	//SpringArm->CameraRotationLagSpeed = 7.f;
	//SpringArm->bInheritPitch = false;
	//SpringArm->bInheritRoll = false;



	MoveModeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	MoveModeCamera->SetupAttachment(GetMesh(), TEXT("CockPitCamera"));
	//MoveModeCamera->SetRelativeLocation(FVector(230.f, 0.f, 70.f));
	//MoveModeCamera->SetupAttachment(SpringArm, TEXT("CockPitCamera"));
	//MoveModeCamera->bUsePawnControlRotation = false;
	MoveModeCamera->FieldOfView = 90.f;

	FireModeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	FireModeCamera->bUsePawnControlRotation = false;
	FireModeCamera->FieldOfView = 90.f;
	FireModeCamera->SetupAttachment(GetMesh(), TEXT("Body_TR"));
	FireModeCamera->Deactivate();

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/LOTAssets/TankAssets/Audio/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<AActor> CrossHairBP(TEXT("/Game/Blueprints/crossBP2.crossBP2_C"));
	CrossHair = CreateDefaultSubobject<UChildActorComponent>("CrossHair");

	if (CrossHairBP.Class != NULL)
	{
		CrossHair->SetChildActorClass(CrossHairBP.Class);
		//CrossHair->SetupAttachment(BarrelMesh, TEXT("Muzzle"));
		CrossHair->SetupAttachment(FireModeCamera);
		CrossHair->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
		CrossHair->SetVisibility(false, true);
		//CrossHair->CreateChildActor();
	}

	static ConstructorHelpers::FClassFinder<AActor> CockPitBP(TEXT("/Game/LOTAssets/CockPit/BP/CockpitBP.CockpitBP_C"));
	CockPit = CreateDefaultSubobject<UChildActorComponent>("CockPit");
	if (CockPitBP.Class != NULL)
	{
		CockPit->SetChildActorClass(CockPitBP.Class);
		CockPit->SetRelativeLocation(FVector(250.0f, 0.0f, -50.0f));
		CockPit->SetupAttachment(GetMesh());
		//CockPit->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	bIsFireMode = false;
	bIsPushFire = false;

	MinShootingPower = 100.f;
	RaisingRate = 50.f;
	MaxShootingPower = 80000.f;
	//�߻� �Ŀ�
	CurShootingPower = MinShootingPower;
	bIsShoot = false;

	CurForwardVal = 0.f;

	CurRightVal = 0.f;

	ExForwardVal = 0.f;

	ExRightVal = 0.f;

	bIsSendForward = false;

	bIsSendRight = false;

	MoveAP = 1.0f;

	CurInventoryIndex = 0;

	Type = 0;

	bIsFireHoming = false;

	for (int i = 0; i < 4; i++) {
		HadProjectileNum[i] = 0;
	}

	bIsMyTurn = false;
}

void ALOTMultiPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetDefaultInvetory();
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	EngineSoundComponent->Play();
	

}
void ALOTMultiPlayer::SetDefaultInvetory()
{
	if (ProjectileInventory.Num() == 0)
	{
		ProjectileInventory.AddUnique(ACommonProjectile::StaticClass());
		ProjectileInventory.AddUnique(AArmorPiercingProjectile::StaticClass());
		ProjectileInventory.AddUnique(AHomingProjectile::StaticClass());
		ProjectileInventory.AddUnique(AEmpProjectile::StaticClass());
		CurrentProjectile = ProjectileInventory[CurInventoryIndex];

	}
}

void ALOTMultiPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);
	InputComponent->BindAxis("Forward", this, &ALOTMultiPlayer::MoveForward);
	InputComponent->BindAxis("Right", this, &ALOTMultiPlayer::MoveRight);
	InputComponent->BindAction("Fire", IE_Released, this, &ALOTMultiPlayer::FireEnd);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ALOTMultiPlayer::FireStart);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTMultiPlayer::FireMode);
	InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ALOTMultiPlayer::ChangePawn);
	InputComponent->BindAction("Q_BT", IE_Pressed, this, &ALOTMultiPlayer::ExWeapon);
	InputComponent->BindAction("E_BT", IE_Pressed, this, &ALOTMultiPlayer::NextWeapon);
	InputComponent->BindAction("Z_BT", IE_Pressed, this, &ALOTMultiPlayer::TurnOver);
	
}



void ALOTMultiPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	d_time = DeltaTime;

	if (bIsMyTurn) {
		if (bIsFireMode)
		{
			ChangeFiremodeBody();

		}

		if (bIsPushFire && !bIsShoot)
		{
			DrawTrajectory();
			RaisePower();
			//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("�����Ŀ� %f"), CurShootingPower));
		}
	}

	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter("RPM", GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
	if (GetController())
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("�����Ŀ� d")));
	}
	



	
}

void ALOTMultiPlayer::ChangeFiremodeBody()
{
	TurretMesh->SetRelativeRotation(FRotator(0.0f, FireModeCamera->RelativeRotation.Yaw, 0.0f));
	BarrelMesh->SetRelativeRotation(FRotator(FireModeCamera->RelativeRotation.Pitch, 0.0f, 0.0f));
	//CrossHair->SetRelativeRotation(FRotator(FireModeCamera->RelativeRotation.Pitch, FireModeCamera->RelativeRotation.Yaw, 0.0f));

}

void ALOTMultiPlayer::SetbIsShoot(bool IsShoot)
{
	bIsShoot = IsShoot;
	bIsFireHoming = false;
}



void ALOTMultiPlayer::FireMode()
{
	ClearBeam();
	if (bIsFireMode == false)
	{
		bIsFireMode = true;
		ChangeCamera(bIsFireMode);
	}
	else
	{
		bIsFireMode = false;
		ChangeCamera(bIsFireMode);
	}

}

void ALOTMultiPlayer::FireStart()
{

	if (CurInventoryIndex == P_COMMON || HadProjectileNum[CurInventoryIndex] > 0) {
		AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (bIsFireMode == true && GameModeTest->bIsMyTurn && !GameModeTest->MyPlayer.Dead && bIsShoot == false && !bIsPushFire && bIsMyTurn)
		{
			bIsPushFire = true;
			CurShootingPower = MinShootingPower;

		}
	}
}

void ALOTMultiPlayer::FireEnd()
{

	if (CurInventoryIndex == P_COMMON || HadProjectileNum[CurInventoryIndex] > 0) {
		ClearBeam();
		AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (CurrentProjectile != NULL && bIsPushFire && GameModeTest->bIsMyTurn && !bIsShoot && bIsMyTurn)
		{
			UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
			ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());

			bIsShoot = true;
			FRotator SpawnRotation = GetActorRotation() + FireModeCamera->RelativeRotation;//
			SpawnRotation = MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation());
			UWorld* const World = GetWorld();
			if (World != NULL)
			{

				const FVector InitialVelocity = UKismetMathLibrary::TransformDirection(UKismetMathLibrary::MakeTransform(SpawnLocation,
					FRotator(0.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f)), FVector(CurShootingPower, 0.f, 0.f));

				TempActor = World->SpawnActor<AProjectile>(CurrentProjectile, SpawnLocation, SpawnRotation);
				TempActor->SetInitialVelocity(InitialVelocity);
				TempActor->ParentTank = this;
				TempActor->SetEnemyFire(false);
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), UTankCameraShake::StaticClass(), GetActorLocation(), 0.f, 500.f, false);
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(TempActor, 1.f, VTBlend_EaseOut, 2.0f, true);
				if (ACommonProjectile* const ProjectileType = Cast<ACommonProjectile>(TempActor))
				{
					Type = PROJECTILE_COMMON;
					TestInstance->SendFire(SpawnLocation, SpawnRotation, CurShootingPower, Type);
					//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź �߻� %d"), Type));
				}
				else if (AArmorPiercingProjectile* const ProjectileType = Cast<AArmorPiercingProjectile>(TempActor))
				{
					Type = PROJECTILE_ARMORPIERCING;
					TestInstance->SendFire(SpawnLocation, SpawnRotation, CurShootingPower, Type);
					//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź �߻�%d"), Type));
				}
				else if (AHomingProjectile* const ProjectileType = Cast<AHomingProjectile>(TempActor))
				{
					Type = PROJECTILE_HOMING;
					//AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					if (GameModeTest->MyPlayer.TargetActor != NULL) {
						ProjectileType->SetHomingTarget(GameModeTest->MyPlayer.TargetActor, CurShootingPower * 5);
						GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Ÿ�� ����ź �߻�%d"), Type));
						bIsFireHoming = true;
					}
					else
						GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź �߻�%d"), Type));

					TestInstance->SendFire(SpawnLocation, SpawnRotation, CurShootingPower, Type);

				}
				else if (AEmpProjectile* const ProjectileType = Cast<AEmpProjectile>(TempActor))
				{
					Type = PROJECTILE_EMP;
					TempActor->ParentDrone = GameModeTest->MyPlayer.Drone;
					TestInstance->SendFire(SpawnLocation, SpawnRotation, CurShootingPower, Type);
				}

				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/TankFire_Cue.TankFire_Cue")),
					SpawnLocation, SpawnRotation);

			}

			if (CurInventoryIndex != P_COMMON) {
				HadProjectileNum[CurInventoryIndex] -= 1;
			}

			if (CurInventoryIndex == P_COMMON) {
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("�Ϲ�ź �߻�")));
			}
			else if (CurInventoryIndex == P_ARMORPIERCE) {
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź �߻� %d�� ����"), HadProjectileNum[CurInventoryIndex]));
			}
			else if (CurInventoryIndex == P_HOMING) {
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź �߻� %d�� ����"), HadProjectileNum[CurInventoryIndex]));
			}
			else if (CurInventoryIndex == P_EMP) {
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("EMP �߻� %d�� ����"), HadProjectileNum[CurInventoryIndex]));
			}
			bIsMyTurn = false;
		}

		else if (bIsFireHoming)
		{
			if (AHomingProjectile* const ProjectileType = Cast<AHomingProjectile>(TempActor))
			{
				ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());
				TestInstance->SendActivateHoming();
				ProjectileType->ActivateHoming();
			}
		}




	}
}

void ALOTMultiPlayer::ChangeCamera(bool bIsFireMode)
{

	if (bIsFireMode == true)
	{
		MoveModeCamera->Deactivate();
		//CockPit->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		FireModeCamera->Activate();
		//CockPit->AttachToComponent(FireModeCamera,FAttachmentTransformRules::KeepRelativeTransform);
		CockPit->SetVisibility(false, true);
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		TurretMesh->SetVisibility(false, false);
		GetMesh()->SetVisibility(false, false);
		BarrelMesh->SetVisibility(false, false);
		CrossHair->SetVisibility(true, true);
		

	}
	else if (bIsFireMode == false)
	{
		FireModeCamera->Deactivate();
		//UI->DetachFromParent(true, true);
		//CockPit->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		MoveModeCamera->Activate();
		//CockPit->AttachToComponent(MoveModeCamera, FAttachmentTransformRules::KeepRelativeTransform);
		CockPit->SetVisibility(true, true);
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		TurretMesh->SetVisibility(true, false);
		GetMesh()->SetVisibility(false, false);
		BarrelMesh->SetVisibility(true, false);
		CrossHair->SetVisibility(false, true);
		
	}
}
//
//
//
//
//
//
void ALOTMultiPlayer::MoveForward(float Val)
{
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	/*if (!bIsFireMode && GameModeTest->bIsMyTurn && GameModeTest->MyPlayer.Moveable && !GameModeTest->MyPlayer.Dead) {
		GetVehicleMovementComponent()->SetThrottleInput(Val);
		if (Val != 0.f)
		{
			GameModeTest->MyPlayer.AP -= MoveAP * d_time;
			if (GameModeTest->MyPlayer.AP <= 0) 
				GameModeTest->MyPlayer.AP = 0;
			
		}
	}
	else*/
		//GetVehicleMovementComponent()->SetThrottleInput(0.f);
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}


void ALOTMultiPlayer::MoveRight(float Val)
{
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	//if (!bIsFireMode && GameModeTest->bIsMyTurn && GameModeTest->MyPlayer.Moveable && !GameModeTest->MyPlayer.Dead) {
	//	GetVehicleMovementComponent()->SetSteeringInput(Val);
	//	
	//}
	//else
		//GetVehicleMovementComponent()->SetThrottleInput(0.f);
	GetVehicleMovementComponent()->SetSteeringInput(Val);

}

void ALOTMultiPlayer::ChangePawn()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Test->SetViewTargetWithBlend(GameModeTest->MyPlayer.Drone, 1.0f, VTBlend_EaseInOut, 10.f, true);
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = "PossessCall";
	LatentActionInfo.UUID = 123;
	LatentActionInfo.Linkage = 0;

	UKismetSystemLibrary::Delay(GetWorld(), 1.f, LatentActionInfo);
	

}

void ALOTMultiPlayer::PossessCall()
{
	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	SetUI(false);
	GameModeTest->MyPlayer.Drone->SetUI(true);
	Test->Possess(GameModeTest->MyPlayer.Drone);
	UGameplayStatics::SpawnSound2D(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Engine/VREditor/Sounds/VR_teleport_Cue.VR_teleport_Cue")));
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Possess call!!!")));
}




void ALOTMultiPlayer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		;
	}
}



FVector ALOTMultiPlayer::GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time)
{
	return StartLocation + (InitialVelocity*time) + (time*time*0.5f*Gravity);
}

void ALOTMultiPlayer::DrawTrajectory()
{
	ClearBeam();
	const FRotator SpawnRotation = GetActorRotation() + FireModeCamera->RelativeRotation;//

	const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation());

	const FVector InitialVelocity = UKismetMathLibrary::TransformDirection(UKismetMathLibrary::MakeTransform(SpawnLocation,
		SpawnRotation, FVector(1.f, 1.f, 1.f)), FVector(CurShootingPower, 0.f, 0.f));

	const float PathLifetime = 5.0f;
	const float TimeInterval = 0.05f;
	const int32 FloorTime = UKismetMathLibrary::FFloor(PathLifetime / TimeInterval);

	float time1, time2;
	FVector point1, point2;

	//Ÿ�ټ��� ������ Ÿ���� ���� �迭
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	//����ŬŸ�Ը� üũ�ϵ��� �Ѵ�.

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	//���� ���带 �����´�.
	UWorld* const World = GetWorld();

	FHitResult OutHit;



	for (int32 index = 0; index < FloorTime; ++index)
	{
		time1 = index * TimeInterval;
		time2 = (index + 1) * TimeInterval;
		point1 = GetSegmentatTime(SpawnLocation, InitialVelocity, FVector(0.f, 0.f, -980.f), time1);
		point2 = GetSegmentatTime(SpawnLocation, InitialVelocity, FVector(0.f, 0.f, -980.f), time2);
		DrawBeam(point1, point2);

		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), point1, point2, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, OutHit, true))
			break;

	}
}

void ALOTMultiPlayer::DrawBeam(FVector StartLocation, FVector EndLocation)
{
	UParticleSystemComponent* TrajectoryBeam = NewObject<UParticleSystemComponent>(this);
	TrajectoryBeam->RegisterComponent();
	TrajectoryBeam->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Particles/PT_ArcingAim.PT_ArcingAim")));

	TrajectoryBeam->SetBeamSourcePoint(0, StartLocation, 0);
	TrajectoryBeam->SetBeamTargetPoint(0, EndLocation, 0);
	BeamArray.Add(TrajectoryBeam);

}

void ALOTMultiPlayer::RaisePower()
{
	if (CurShootingPower<MaxShootingPower)
		CurShootingPower += RaisingRate;
}

void ALOTMultiPlayer::ClearBeam()
{
	for (auto&i : BeamArray)
	{
		i->DestroyComponent();
	}
	BeamArray.Empty();
}

void ALOTMultiPlayer::SetUI(bool bIsPlayer)
{
	if (!bIsPlayer)
	{
		GetMesh()->SetVisibility(true, true);
		CockPit->SetVisibility(false, true);
		
	}
	else {
		GetMesh()->SetVisibility(true, false);
		CockPit->SetVisibility(true, true);
	}
	
}

void ALOTMultiPlayer::SetDead()
{
	TurretMesh->SetSimulatePhysics(true);
	BarrelMesh->SetSimulatePhysics(true);
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


void ALOTMultiPlayer::NextWeapon()
{
	if (3 > CurInventoryIndex) {
		CurInventoryIndex++;
		CurrentProjectile = ProjectileInventory[CurInventoryIndex];
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� ��ź %d"), CurInventoryIndex));
	if (CurInventoryIndex == P_COMMON) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("�Ϲ�ź")));
	}
	else if (CurInventoryIndex == P_ARMORPIERCE) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź %d"), HadProjectileNum[CurInventoryIndex]));
	}
	else if (CurInventoryIndex == P_HOMING) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź %d"), HadProjectileNum[CurInventoryIndex]));
	}
	else if (CurInventoryIndex == P_EMP) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("EMP %d"), HadProjectileNum[CurInventoryIndex]));
	}
}

void ALOTMultiPlayer::ExWeapon()
{
	if (0 < CurInventoryIndex) {
		CurInventoryIndex--;
		CurrentProjectile = ProjectileInventory[CurInventoryIndex];
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� ��ź %d"), CurInventoryIndex));
	if (CurInventoryIndex == P_COMMON) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("�Ϲ�ź")));
	}
	else if (CurInventoryIndex == P_ARMORPIERCE) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź %d"), HadProjectileNum[CurInventoryIndex]));
	}
	else if (CurInventoryIndex == P_HOMING) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("����ź %d"), HadProjectileNum[CurInventoryIndex]));
	}
	else if (CurInventoryIndex == P_EMP) {
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("EMP %d"), HadProjectileNum[CurInventoryIndex]));
	}
}

void ALOTMultiPlayer::TurnOver()
{
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());
	if (!bIsFireMode && GameModeTest->bIsMyTurn && GameModeTest->MyPlayer.Moveable && !GameModeTest->MyPlayer.Dead)
	{
		if (GameModeTest->MyPlayer.AP>200.f)
		{
			GameModeTest->MyPlayer.AP -= 200.f;
			GetMesh()->AddImpulse(FVector(0.f,0.f, 2000000.f));
			GetMesh()->AddRelativeRotation(FRotator(180.f,0.f,0.f), false, nullptr, ETeleportType::TeleportPhysics);

		}
	}
}

void ALOTMultiPlayer::SetArmorPierceNum() {
	HadProjectileNum[P_ARMORPIERCE] += 2;
}

void ALOTMultiPlayer::SetHomingNum() {
	HadProjectileNum[P_HOMING] += 1;
}

void ALOTMultiPlayer::SetEmpNum() {
	HadProjectileNum[P_EMP] += 1;
}