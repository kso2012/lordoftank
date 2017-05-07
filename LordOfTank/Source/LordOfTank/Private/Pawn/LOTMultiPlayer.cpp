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
#include "GameMode/MultiGameMode.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "LOTDrone.h"
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
	if (CrossHairBP.Class != NULL)
	{
		CrossHair->SetChildActorClass(CrossHairBP.Class);
		CrossHair->SetupAttachment(FireModeCamera);
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



	//CrossHair->attach

	/*if (CrossHairBP.Class != NULL)
	{
	CrossHair = CrossHairBP.Class;

	}*/


	bIsFireMode = false;
	bIsPushFire = false;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	// �ʱ� AP�� 100���� ����
	AP = 100.f;

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

}

void ALOTMultiPlayer::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();

	EngineSoundComponent->Play();

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

	InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ALOTMultiPlayer::ChangePawn);

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

		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("�����Ŀ� %f"), CurShootingPower));
	}


	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter("RPM", GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);




	
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
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		TurretMesh->SetVisibility(false, false);
		GetMesh()->SetVisibility(false, false);
		BarrelMesh->SetVisibility(false, false);
		CrossHair->SetVisibility(true, true);
		UI->SetupAttachment(FireModeCamera);
	}
	else if (bIsFireMode == false)
	{
		MoveModeCamera->Activate();
		FireModeCamera->Deactivate();
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		TurretMesh->SetVisibility(true, false);
		GetMesh()->SetVisibility(true, false);
		BarrelMesh->SetVisibility(true, false);
		CrossHair->SetVisibility(false, true);
		UI->SetupAttachment(MoveModeCamera);
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
	ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());
	

	if (!bIsFireMode && TestInstance->bIsmyTurn) {
		GetVehicleMovementComponent()->SetThrottleInput(Val);
	}
	else
		GetVehicleMovementComponent()->SetThrottleInput(0.f);
}


void ALOTMultiPlayer::MoveRight(float Val)
{
	ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(GetGameInstance());

	if (!bIsFireMode && TestInstance->bIsmyTurn) {
		GetVehicleMovementComponent()->SetSteeringInput(Val);
	}
	else
		GetVehicleMovementComponent()->SetThrottleInput(0.f);

}

void ALOTMultiPlayer::ChangePawn()
{
	
	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(getgamemode);
	Test->Possess(GameModeTest->MyPlayer.Drone);

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


