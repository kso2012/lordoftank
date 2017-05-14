// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Vehicle/FrontWheel.h"
#include "Vehicle/RearWheel.h"
#include "Weapon/Projectile.h"
#include "Weapon/CommonProjectile.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/HomingProjectile.h"
#include "Effects/TankCameraShake.h"
#include "LOTGameInstance.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "LordOfTank/LordOfTankGameModeBase.h"
#include "LOTDrone.h"
#include "LOTPlayer.h"

#define PawnTank 1
#define PawnDrone 2

#define Nope 0
#define CorrectAim 1
#define InCorrectAim 2



ALOTPlayer::ALOTPlayer()
{
	
	PrimaryActorTick.bCanEverTick = true;
	//스켈레톤컴포넌트에 메쉬 적용.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySkeletalMesh(TEXT("/Game/LOTAssets/TankAssets/LOTBody.LOTBody"));
	GetMesh()->SetSkeletalMesh(BodySkeletalMesh.Object);
	//스켈레톤컴포넌트에 애니메이션 적용.
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/LOTAssets/TankAssets/LOTPlaytankAnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	//GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ALOTPlayer::OnOverlapBegin);
	
	//터렛컴포넌트에 메쉬 적용.
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretStaticMesh(TEXT("/Game/LOTAssets/TankAssets/Meshes/LBX1Turret_SM"));
	TurretMesh->SetStaticMesh(TurretStaticMesh.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> TurretMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	//TurretMesh->SetMaterial(0,TurretMaterial.Object);
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
	//총구에 씬컴포넌트 부착.
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//MuzzleLocation->AttachToComponent(BarrelMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	MuzzleLocation->SetupAttachment(BarrelMesh, TEXT("Muzzle"));

	
	// 바퀴에 휠 클래스 적용
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Front_RW");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Front_LW");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = URearWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Rear_RW");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = URearWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Rear_LW");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;



	MoveModeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	MoveModeCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	MoveModeCamera->bUsePawnControlRotation = false;
	MoveModeCamera->FieldOfView = 90.f;

	FireModeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	FireModeCamera->bUsePawnControlRotation = false;
	FireModeCamera->FieldOfView = 90.f;
	//FireModeCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	FireModeCamera->SetupAttachment(GetMesh(), TEXT("Body_TR"));
	FireModeCamera->Deactivate();

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/LOTAssets/TankAssets/Audio/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(GetMesh());
	
	static ConstructorHelpers::FClassFinder<AActor> CrossHairBP(TEXT("/Game/Blueprints/crossBP.crossBP_C"));
	CrossHair = CreateDefaultSubobject<UChildActorComponent>("CrossHair");
	if (CrossHairBP.Class != NULL){
		CrossHair->SetChildActorClass(CrossHairBP.Class);
		CrossHair->SetupAttachment(GetMesh(), TEXT("Body_TR"));
		CrossHair->SetVisibility(false, true);
		//CrossHair->CreateChildActor();
	}
	

	static ConstructorHelpers::FClassFinder<AActor> UIBP(TEXT("/Game/Blueprints/UIBP.UIBP_C"));
	UI = CreateDefaultSubobject<UChildActorComponent>("UI");
	if (UIBP.Class != NULL)
	{
		UI->SetChildActorClass(UIBP.Class);
		UI->SetupAttachment(MoveModeCamera);
	}

	
	
	bIsFireMode = false;
	bIsPushFire = false;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	// 초기 AP를 100으로 설정
	AP = 2500.f;

	MinShootingPower=100.f;
	RaisingRate = 50.f;
	MaxShootingPower= 80000.f;
	//발사 파워
	CurShootingPower= MinShootingPower;
	PossessTank = true;

	isNotAI = true;
	bIsShoot = false;

	TurretAim = Nope;
	RightShot = false;
	AimCount = 0;
	bIsWaiting = false;
	bIsTestShot = false;
	MoveAP = 1.0f;
	timercounteron = false;
}

void ALOTPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetDefaultInvetory();
	OnResetVR();
	

}


void ALOTPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);
	InputComponent->BindAxis("Forward", this, &ALOTPlayer::MoveForward);
	InputComponent->BindAxis("Right", this, &ALOTPlayer::MoveRight);
	InputComponent->BindAction("Fire", IE_Released, this, &ALOTPlayer::FireEnd);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ALOTPlayer::FireStart);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTPlayer::FireMode);

	InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ALOTPlayer::ChangePawn);

}



void ALOTPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFireMode)
	{
		ChangeFiremodeBody();
		
	}

	if (bIsPushFire)
	{
		DrawTrajectory();
		RaisePower();
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("현재파워 %f"), CurShootingPower));
	}

	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter("RPM", GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), myTurn));
	

	if (timercounteron) {
		timercounter += DeltaTime;
		if (timercounter > 1.0f) {
			DronePossess();
			timercounter = 0;
			timercounteron = false;
		}
	}
}

void ALOTPlayer::ChangeFiremodeBody()
{
	TurretMesh->SetRelativeRotation(FRotator(0.0f, FireModeCamera->RelativeRotation.Yaw, 0.0f));
	BarrelMesh->SetRelativeRotation(FRotator(FireModeCamera->RelativeRotation.Pitch, 0.0f, 0.0f));
	CrossHair->SetRelativeRotation(FRotator(FireModeCamera->RelativeRotation.Pitch, FireModeCamera->RelativeRotation.Yaw, 0.0f));

}

void ALOTPlayer::FireMode()
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

void ALOTPlayer::FireStart()
{
	if (bIsFireMode == true)
	{
		bIsPushFire = true;
		CurShootingPower = MinShootingPower;
		bIsShoot = true;
	}
}


void ALOTPlayer::FireEnd()
{
	bIsPushFire = false;
	ClearBeam();
	if (CurrentProjectile != NULL && bIsFireMode )
	{
		FRotator SpawnRotation = GetActorRotation()+ FireModeCamera->RelativeRotation;//

		if (!isNotAI) {
			SpawnRotation = MuzzleLocation->GetComponentRotation();
		}
		
		const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) ;

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());
			const FVector InitialVelocity = UKismetMathLibrary::TransformDirection(UKismetMathLibrary::MakeTransform(SpawnLocation,
				FRotator(0.f,0.f,0.f), FVector(1.f, 1.f, 1.f)), FVector(CurShootingPower, 0.f, 0.f));


			AProjectile* TempActor = World->SpawnActor<AProjectile>(CurrentProjectile, SpawnLocation, SpawnRotation);
			TempActor->SetInitialVelocity(InitialVelocity);


			//World->SpawnActor<AProjectile>(CurrentProjectile, SpawnLocation, SpawnRotation)->SetHomingTarget(HomingTarget);
			//// spawn the pickup
			//APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			//World->SpawnActor<ALOTDrone>(ALOTDrone::StaticClass(), SpawnLocation+FVector(0.0f,0.0f,1000.f), SpawnRotation);
			
			if (isNotAI) {
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), UTankCameraShake::StaticClass(), GetActorLocation(), 0.f, 500.f, false);
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(TempActor, 0.4f, VTBlend_Linear, 0.0f, true);
				
			}
			TempActor->GetTank(this);
		}
	}
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("배열길이 %d"), ProjectileInventory.Num()));
	FireMode();
	if(isNotAI)
		CurShootingPower = MinShootingPower;

}

void ALOTPlayer::ChangeCamera(bool bIsFireMode)
{
	
	if (isNotAI) {
		if (bIsFireMode == true)
		{
			MoveModeCamera->Deactivate();
			UI->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			FireModeCamera->Activate();
			UI->AttachToComponent(FireModeCamera, FAttachmentTransformRules::KeepRelativeTransform);
			//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
			TurretMesh->SetVisibility(false, false);
			GetMesh()->SetVisibility(false, false);
			BarrelMesh->SetVisibility(false, false);
			CrossHair->SetVisibility(true, true);
		}
		else if (bIsFireMode == false)
		{
			MoveModeCamera->Activate();
			UI->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			FireModeCamera->Deactivate();
			UI->AttachToComponent(MoveModeCamera, FAttachmentTransformRules::KeepRelativeTransform);
			//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
			TurretMesh->SetVisibility(true, false);
			GetMesh()->SetVisibility(true, false);
			BarrelMesh->SetVisibility(true, false);
			CrossHair->SetVisibility(false, true);
		}
	}
}




void ALOTPlayer::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void ALOTPlayer::MoveForward(float Val)
{
	//SetVehicleMovement();
	if (!bIsFireMode && AP > 0) {
		GetVehicleMovementComponent()->SetThrottleInput(Val);
		
		ULOTGameInstance* const Test = Cast<ULOTGameInstance>(GetGameInstance());
		//GetVehicleMovementComponent()->set
		//Test->SendPos(GetActorLocation(),GetVehicleMovementComponent());
		//ALOTPlayer* const Test = Cast<ALOTPlayer>(InsideActor)
		if(Val)
			AP -= MoveAP;
	}
}


void ALOTPlayer::MoveRight(float Val)
{
	if (!bIsFireMode && AP > 0) {
		GetVehicleMovementComponent()->SetSteeringInput(Val);
		if(Val)
			AP -= MoveAP;
	}
	GetMesh();

}

void ALOTPlayer::SetDefaultInvetory()
{
	if (ProjectileInventory.Num() == 0)
	{
		ProjectileInventory.AddUnique(ACommonProjectile::StaticClass());
		ProjectileInventory.AddUnique(AArmorPiercingProjectile::StaticClass());
		ProjectileInventory.AddUnique(AHomingProjectile::StaticClass());
		CurrentProjectile = ProjectileInventory[0];

	}
}



//void ALOTPlayer::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// Other Actor is the actor that triggered the event. Check that is not ourself.  
//	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
//	{
//		// Turn off the light  
//		//PointLight->SetVisibility(false);
//	}
//}

void ALOTPlayer::ApplyDamage(float damage)
{
	CurrentHealth -= damage;
	if (CurrentHealth <= 0.f) {

		TurretMesh->SetSimulatePhysics(true);
		BarrelMesh->SetSimulatePhysics(true);
		TurretMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BarrelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	}
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CurrentHealth));
}

FVector ALOTPlayer::GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time)
{
	return StartLocation + (InitialVelocity*time) + (time*time*0.5f*Gravity);
}

void ALOTPlayer::DrawTrajectory()
{
	ClearBeam();
	const FRotator SpawnRotation = GetActorRotation() + FireModeCamera->RelativeRotation;//

	const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation());

	const FVector InitialVelocity = UKismetMathLibrary::TransformDirection(UKismetMathLibrary::MakeTransform(SpawnLocation,
		SpawnRotation, FVector(1.f, 1.f, 1.f)), FVector(CurShootingPower, 0.f, 0.f));

	const float PathLifetime = 5.0f;
	const float TimeInterval = 0.05f;
	const int32 FloorTime = UKismetMathLibrary::FFloor(PathLifetime/ TimeInterval);

	float time1, time2;
	FVector point1, point2;

	//타겟설정 가능한 타입을 넣을 배열
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	//비히클타입만 체크하도록 한다.
	
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	//현재 월드를 가져온다.
	UWorld* const World = GetWorld();

	FHitResult OutHit;
	
	
	//for (int32 index = 0; index < 3; ++index)
	for (int32 index = 0; index < FloorTime; ++index)
	{
		time1 = index * TimeInterval;
		time2 = (index + 1) * TimeInterval;
		DrawBeam(point1, point2);

		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), point1, point2, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, OutHit, true)) {
			//RightShot = true;
			//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("crash")));
			break;
		}
		
	}
}

void ALOTPlayer::DrawBeam(FVector StartLocation, FVector EndLocation)
{
	UParticleSystemComponent* TrajectoryBeam = NewObject<UParticleSystemComponent>(this);
	TrajectoryBeam->RegisterComponent();
	TrajectoryBeam->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Particles/PT_ArcingAim.PT_ArcingAim")));

	TrajectoryBeam->SetBeamSourcePoint(0, StartLocation, 0);
	TrajectoryBeam->SetBeamTargetPoint(0, EndLocation, 0);
	BeamArray.Add(TrajectoryBeam);
	
}

void ALOTPlayer::RaisePower()
{
	if(CurShootingPower<MaxShootingPower)
		CurShootingPower += RaisingRate;
}

void ALOTPlayer::ClearBeam()
{
	for (auto&i : BeamArray)
	{
		i->DestroyComponent();
	}
	BeamArray.Empty();
}

void ALOTPlayer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		;
	}
}

void ALOTPlayer::ChangePawn()
{
	if (PossessTank)
		PossessTank = false;

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ALordOfTankGameModeBase* const GameModeTest = Cast<ALordOfTankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Test->SetViewTargetWithBlend(GameModeTest->MyPlayer.Drone, 1.0f, VTBlend_EaseInOut, 10.f, true);
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = "DronePossess";
	LatentActionInfo.UUID = 999;
	LatentActionInfo.Linkage = 0;
	timercounter = 0;
	timercounteron = true;
}


void ALOTPlayer::DronePossess()
{
	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ALordOfTankGameModeBase* const GameModeTest = Cast<ALordOfTankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	SetSingleUI(false);
	GameModeTest->MyPlayer.Drone->SetSingleUI(true);
	Test->UnPossess();
	Test->Possess(GameModeTest->MyPlayer.Drone);
	GameModeTest->MyPlayer.Tank->DisableInput(Test);
	GameModeTest->MyPlayer.Drone->EnableInput(Test);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Possess call!!!")));
}

void ALOTPlayer::SetSingleUI(bool bIsPlayer)
{
	if (!bIsPlayer)
	{
		UI->SetVisibility(false, true);
	}
	else
		UI->SetVisibility(true, true);


}

//void ALOTPlayer::ToCallSetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent)
//{
//	if (MovementComponent)
//	{
//		SetVehicleMovement();
//	}
//	//SetVehicleMovement(MovementComponent);
//	
//}


void ALOTPlayer::ChangeTurn() {
		if (myTurn)
			myTurn = false;
		else
			myTurn = true;
		// 발사중일 땐 bIsShoot = true인데 포탄이 폭발할 때 턴을 넘기도록 함. 이 함수는 Projectile에서 호출됨
		bIsShoot = false; 
		RightShot = false;
		AP += 20;
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Trun Changed")));
}


void ALOTPlayer::ShootAI(float power) {
	if (myTurn && !isNotAI && !bIsShoot) {
		bIsFireMode = true;
		bIsShoot = true;
		//CurShootingPower = power;
		if(RightShot)
			AimCount = 0;
		FireEnd();
	}
}


void ALOTPlayer::TurnAI() {
	MoveRight(1);
	MoveForward(1);
}



void ALOTPlayer::RotateTurret(FRotator RotateDirection) {
	if (TurretAim == Nope) {
		TurretMesh->SetWorldRotation(RotateDirection);
	}
}

void ALOTPlayer::SetAim(float distance) {
	if (AimCount == 0) {
		CurShootingPower = distance - 10000;
		AimCount++;
		bIsTestShot = true;
	}
	if (!bIsWaiting) {
		CurShootingPower += 1000;

		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Power = %f"), CurShootingPower));
		bIsWaiting = true;
		ShootAI(CurShootingPower);
	}


	if ((CurShootingPower > distance + 30000.f || CurShootingPower > MaxShootingPower) && !RightShot) {
		CurShootingPower = MinShootingPower;
		BarrelMesh->AddLocalRotation(FRotator(1.f, 0, 0));
		if (BarrelMesh->GetComponentRotation().Roll > 80.f)
			BarrelMesh->AddLocalRotation(FRotator(-1.f, 0, 0));
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Barrel Rotate")));
	}

}
