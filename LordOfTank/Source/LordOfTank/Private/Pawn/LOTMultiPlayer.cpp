// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Weapon/Projectile.h"
#include "Weapon/CommonProjectile.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/HomingProjectile.h"
#include "Effects/TankCameraShake.h"
#include "LOTGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "LOTDrone.h"
#include "LOTMultiPlayer.h"




ALOTMultiPlayer::ALOTMultiPlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySkeletalMesh(TEXT("/Game/LOTAssets/TankAssets/LOTBody.LOTBody"));
	BodyMesh->SetSkeletalMesh(BodySkeletalMesh.Object);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BodyMesh->OnComponentHit.AddDynamic(this, &ALOTMultiPlayer::OnHit);
	//BodyMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	//BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BodyMesh->OnComponentBeginOverlap.AddDynamic(this, &ALOTMultiPlayer::OnOverlapBegin);
	RootComponent = BodyMesh;

	//터렛컴포넌트에 메쉬 적용.
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretStaticMesh(TEXT("/Game/LOTAssets/TankAssets/Meshes/LBX1Turret_SM"));
	TurretMesh->SetStaticMesh(TurretStaticMesh.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> TurretMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	//TurretMesh->SetMaterial(0, TurretMaterial.Object);
	//TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	TurretMesh->SetupAttachment(BodyMesh, TEXT("Body_TR"));
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
	//FireModeCamera->AttachToComponent(BodyMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	FireModeCamera->SetupAttachment(BodyMesh, TEXT("Body_TR"));
	FireModeCamera->Deactivate();

	/*static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/LOTAssets/TankAssets/Audio/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(BodyMesh);*/

	static ConstructorHelpers::FClassFinder<AActor> CrossHairBP(TEXT("/Game/Blueprints/crossBP.crossBP_C"));
	CrossHair = CreateDefaultSubobject<UChildActorComponent>("CrossHair");
	if (CrossHairBP.Class != NULL)
	{
		CrossHair->SetChildActorClass(CrossHairBP.Class);
		CrossHair->SetupAttachment(FireModeCamera);
		CrossHair->SetVisibility(false, true);
		//CrossHair->CreateChildActor();
	}
	/*static ConstructorHelpers::FClassFinder<AActor> UIBP(TEXT("/Game/Blueprints/UIBP.UIBP_C"));
	UI = CreateDefaultSubobject<UChildActorComponent>("UI");
	if (UIBP.Class != NULL)
	{
		UI->SetChildActorClass(UIBP.Class);
		UI->SetupAttachment(MoveModeCamera);
	}*/



	
	//CrossHair->attach

	/*if (CrossHairBP.Class != NULL)
	{
		CrossHair = CrossHairBP.Class;
	
	}*/


	bIsFireMode = false;
	bIsPushFire = false;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	// 초기 AP를 100으로 설정
	AP = 100.f;
	bIsTurn = false;
	MinShootingPower = 100.f;
	RaisingRate = 50.f;
	MaxShootingPower = 80000.f;
	//발사 파워
	CurShootingPower = MinShootingPower;
	bIsShoot = false;

}

void ALOTMultiPlayer::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();

	//EngineSoundComponent->Play();

}


void ALOTMultiPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);
	InputComponent->BindAxis("Forward", this, &ALOTMultiPlayer::MoveForward);
	InputComponent->BindAxis("Right", this, &ALOTMultiPlayer::MoveRight);
	//InputComponent->BindAction("Fire", IE_Released, this, &ALOTMultiPlayer::FireEnd);
	//InputComponent->BindAction("Fire", IE_Pressed, this, &ALOTMultiPlayer::FireStart);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTMultiPlayer::FireMode);

	//InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ALOTMultiPlayer::ChangePawn);

}



void ALOTMultiPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFireMode)
	{
		ChangeFiremodeBody();

	}

	if (bIsPushFire)
	{

		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("현재파워 %f"), CurShootingPower));
	}

	
	//float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	//EngineSoundComponent->SetFloatParameter("RPM", GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);

}

void ALOTMultiPlayer::ChangeFiremodeBody()
{
	TurretMesh->SetRelativeRotation(FRotator(0.0f, FireModeCamera->RelativeRotation.Yaw, 0.0f));
	BarrelMesh->SetRelativeRotation(FRotator(FireModeCamera->RelativeRotation.Pitch, 0.0f, 0.0f));

}

void ALOTMultiPlayer::FireMode()
{

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

//void ALOTMultiPlayer::FireStart()
//{
//	if (bIsFireMode == true)
//	{
//		bIsPushFire = true;
//		CurShootingPower = MinShootingPower;
//	}
//}
//
//
//void ALOTMultiPlayer::FireEnd()
//{
//
//
//}
//
void ALOTMultiPlayer::ChangeCamera(bool bIsFireMode)
{

	if (bIsFireMode == true)
	{
		MoveModeCamera->Deactivate();
		FireModeCamera->Activate();
		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		TurretMesh->SetVisibility(false, false);
		BodyMesh->SetVisibility(false, false);
		BarrelMesh->SetVisibility(false, false);
		CrossHair->SetVisibility(true,true);
		//UI->SetupAttachment(FireModeCamera);
	}
	else if (bIsFireMode == false)
	{
		MoveModeCamera->Activate();
		FireModeCamera->Deactivate();
		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		TurretMesh->SetVisibility(true, false);
		BodyMesh->SetVisibility(true, false);
		BarrelMesh->SetVisibility(true, false);
		CrossHair->SetVisibility(false,true);
		//UI->SetupAttachment(MoveModeCamera);
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


	if (!bIsFireMode && bIsTurn) {
		//GetVehicleMovementComponent()->SetThrottleInput(Val);
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CurrentHealth));
		ULOTGameInstance* const Test = Cast<ULOTGameInstance>(GetGameInstance());
		Test->MoveForward(Val);
		//GetVehicleMovementComponent()->SetThrottleInput(Val);
		//GetVehicleMovementComponent()->SetThrottleInput(Val);



	}
}


void ALOTMultiPlayer::MoveRight(float Val)
{
	if (!bIsFireMode && bIsTurn) {
		ULOTGameInstance* const Test = Cast<ULOTGameInstance>(GetGameInstance());
		Test->MoveRight(Val);
	}
}




//
//void ALOTMultiPlayer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//
//	// Only add impulse and destroy projectile if we hit a physics
//	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
//	{
//		;
//	}
//}



//void ALOTMultiPlayer::ToCallSetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent)
//{
//	if (MovementComponent)
//	{
//		SetVehicleMovement();
//	}
//	//SetVehicleMovement(MovementComponent);
//	
//}





//void ALOTMultiPlayer::ToCallSetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent)
//{
//	if (MovementComponent)
//	{
//		SetVehicleMovement();
//	}
//	//SetVehicleMovement(MovementComponent);
//	
//}


