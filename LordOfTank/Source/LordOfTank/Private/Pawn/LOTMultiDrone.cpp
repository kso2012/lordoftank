// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTMultiDrone.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/MultiGameMode.h"
#include "LOTGameInstance.h"
#include "LOTPlayer.h"




ALOTMultiDrone::ALOTMultiDrone()
{

	PrimaryActorTick.bCanEverTick = true;


	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh1;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh2;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh3;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh4;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh5;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh6;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh7;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh8;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh9;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh10;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh11;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh12;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh13;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh14;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh15;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh16;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh17;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh18;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh19;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh20;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh21;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh22;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BabylonMesh23;
		FConstructorStatics()
			: BabylonMesh(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl1.babylon_Babyl1")),
			BabylonMesh1(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl.babylon_Babyl")),
			BabylonMesh2(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl9.babylon_Babyl9")),
			BabylonMesh3(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babylon4.babylon_Babylon4")),
			BabylonMesh4(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Part_1.babylon_Part_1")),
			BabylonMesh5(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl10.babylon_Babyl10")),
			BabylonMesh6(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl11.babylon_Babyl11")),
			BabylonMesh7(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Part_.babylon_Part_")),
			BabylonMesh8(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Part_1_1.babylon_Part_1_1")),
			BabylonMesh9(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl12.babylon_Babyl12")),
			BabylonMesh10(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl13.babylon_Babyl13")),
			BabylonMesh11(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Part_1_2.babylon_Part_1_2")),
			BabylonMesh12(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_TooBig0.babylon_TooBig0")),
			BabylonMesh13(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl14.babylon_Babyl14")),
			BabylonMesh14(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl15.babylon_Babyl15")),
			BabylonMesh15(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_TooBig1.babylon_TooBig1")),
			BabylonMesh16(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_TooBig2.babylon_TooBig2")),
			BabylonMesh17(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl16.babylon_Babyl16")),
			BabylonMesh18(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl2.babylon_Babyl2")),
			BabylonMesh19(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl3.babylon_Babyl3")),
			BabylonMesh20(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl4.babylon_Babyl4")),
			BabylonMesh21(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl6.babylon_Babyl6")),
			BabylonMesh22(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl5.babylon_Babyl5")),
			BabylonMesh23(TEXT("/Game/LOTAssets/DroneAssets/Babylon/babylon_Babyl8.babylon_Babyl8"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp"));
	// Use a sphere as a simple collision representation
	CollisionComp->InitCapsuleSize(900.f,400.f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Drone");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_Pawn);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = CollisionComp;
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetSimulatePhysics(true);

	BeamCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapSuleComp"));
	BeamCollisionComp->InitCapsuleSize(1000.f, 3000.f);
	BeamCollisionComp->BodyInstance.SetCollisionProfileName("DroneBeam");
	BeamCollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BeamCollisionComp->CanCharacterStepUpOn = ECB_No;
	BeamCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BeamCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BeamCollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	BeamCollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	BeamCollisionComp->SetRelativeLocation(FVector(0.f, 0.f, -3400.f));
	BeamCollisionComp->SetupAttachment(CollisionComp);
	

	BabylonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh"));
	BabylonMesh->SetStaticMesh(ConstructorStatics.BabylonMesh.Get());
	BabylonMesh->SetupAttachment(CollisionComp);
	BabylonMesh->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	

	BabylonMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh1"));
	BabylonMesh1->SetStaticMesh(ConstructorStatics.BabylonMesh1.Get());
	BabylonMesh1->SetupAttachment(BabylonMesh);

	BabylonMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh2"));
	BabylonMesh2->SetStaticMesh(ConstructorStatics.BabylonMesh2.Get());
	BabylonMesh2->SetupAttachment(BabylonMesh1);

	BabylonMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh3"));
	BabylonMesh3->SetStaticMesh(ConstructorStatics.BabylonMesh3.Get());
	BabylonMesh3->SetupAttachment(BabylonMesh2);

	BabylonMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh4"));
	BabylonMesh4->SetStaticMesh(ConstructorStatics.BabylonMesh4.Get());
	BabylonMesh4->SetupAttachment(BabylonMesh3);

	BabylonMesh5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh5"));
	BabylonMesh5->SetStaticMesh(ConstructorStatics.BabylonMesh5.Get());
	BabylonMesh5->SetupAttachment(BabylonMesh4);

	BabylonMesh6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh6"));
	BabylonMesh6->SetStaticMesh(ConstructorStatics.BabylonMesh6.Get());
	BabylonMesh6->SetupAttachment(BabylonMesh5);

	BabylonMesh7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh7"));
	BabylonMesh7->SetStaticMesh(ConstructorStatics.BabylonMesh7.Get());
	BabylonMesh7->SetupAttachment(BabylonMesh6);

	BabylonMesh8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh8"));
	BabylonMesh8->SetStaticMesh(ConstructorStatics.BabylonMesh8.Get());
	BabylonMesh8->SetupAttachment(BabylonMesh7);

	BabylonMesh9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh9"));
	BabylonMesh9->SetStaticMesh(ConstructorStatics.BabylonMesh9.Get());
	BabylonMesh9->SetupAttachment(BabylonMesh8);

	BabylonMesh10 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh10"));
	BabylonMesh10->SetStaticMesh(ConstructorStatics.BabylonMesh10.Get());
	BabylonMesh10->SetupAttachment(BabylonMesh9);

	BabylonMesh11 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh11"));
	BabylonMesh11->SetStaticMesh(ConstructorStatics.BabylonMesh11.Get());
	BabylonMesh11->SetupAttachment(BabylonMesh10);


	BabylonMesh13 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh13"));
	BabylonMesh13->SetStaticMesh(ConstructorStatics.BabylonMesh13.Get());
	BabylonMesh13->SetupAttachment(BabylonMesh11);

	BabylonMesh14 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh14"));
	BabylonMesh14->SetStaticMesh(ConstructorStatics.BabylonMesh14.Get());
	BabylonMesh14->SetupAttachment(BabylonMesh13);

	BabylonMesh15 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh15"));
	BabylonMesh15->SetStaticMesh(ConstructorStatics.BabylonMesh15.Get());
	BabylonMesh15->SetupAttachment(BabylonMesh14);

	BabylonMesh16 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh16"));
	BabylonMesh16->SetStaticMesh(ConstructorStatics.BabylonMesh16.Get());
	BabylonMesh16->SetupAttachment(BabylonMesh15);

	BabylonMesh17 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh17"));
	BabylonMesh17->SetStaticMesh(ConstructorStatics.BabylonMesh17.Get());
	BabylonMesh17->SetupAttachment(BabylonMesh16);

	BabylonMesh18 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh18"));
	BabylonMesh18->SetStaticMesh(ConstructorStatics.BabylonMesh18.Get());
	BabylonMesh18->SetupAttachment(BabylonMesh17);

	BabylonMesh19 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh19"));
	BabylonMesh19->SetStaticMesh(ConstructorStatics.BabylonMesh19.Get());
	BabylonMesh19->SetupAttachment(BabylonMesh18);
	//���� �޽�
	BabylonMesh20 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh20"));
	BabylonMesh20->SetStaticMesh(ConstructorStatics.BabylonMesh20.Get());
	BabylonMesh20->SetupAttachment(BabylonMesh);
	//���� ����
	BabylonMesh21 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh21"));
	BabylonMesh21->SetStaticMesh(ConstructorStatics.BabylonMesh21.Get());
	BabylonMesh21->SetupAttachment(BabylonMesh);
	//����2
	BabylonMesh22 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh22"));
	BabylonMesh22->SetStaticMesh(ConstructorStatics.BabylonMesh22.Get());
	BabylonMesh22->SetupAttachment(BabylonMesh21);

	//������ ���� ����1
	BabylonMesh23 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh23"));
	BabylonMesh23->SetStaticMesh(ConstructorStatics.BabylonMesh23.Get());
	BabylonMesh23->SetupAttachment(BabylonMesh22);

	//������ ���� ����2
	BabylonMesh12 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh12"));
	BabylonMesh12->SetStaticMesh(ConstructorStatics.BabylonMesh12.Get());
	BabylonMesh12->SetupAttachment(BabylonMesh21);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = .0f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;
	//SpringArm->SetRelativeRotation(FRotator(-30.f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(-20.0f, 0.0f, 130.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm);
	//Camera->SetRelativeLocation(FVector(20.0f, 0.0f, 130.0f));
	Camera->bUsePawnControlRotation = false;

	

	SpringArm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm1"));
	SpringArm2->SetupAttachment(RootComponent);
	SpringArm2->TargetArmLength = 0.0f;
	SpringArm2->SocketOffset = FVector(0.f, 0.f, 0.f);
	SpringArm2->bEnableCameraLag = false;
	SpringArm2->CameraLagSpeed = 0.f;

	DetectCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	DetectCamera->SetupAttachment(SpringArm2, USpringArmComponent::SocketName);
	DetectCamera->bUsePawnControlRotation = false;
	DetectCamera->Deactivate();

	BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BeamStaticMesh(TEXT("/Game/Effect/S_EV_SimpleLightBeam_02.S_EV_SimpleLightBeam_02"));
	BeamMesh->SetStaticMesh(BeamStaticMesh.Object);
	BeamMesh->SetupAttachment(CollisionComp);
	BeamMesh->SetRelativeScale3D(FVector(44.408920f, 44.408920f, 46.852932f));
	BeamMesh->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<AActor> CrossHairBP(TEXT("/Game/Blueprints/crossBP.crossBP_C"));
	CrossHair = CreateDefaultSubobject<UChildActorComponent>("CrossHair");
	if (CrossHairBP.Class != NULL)
	{
		CrossHair->SetChildActorClass(CrossHairBP.Class);
		CrossHair->SetupAttachment(DetectCamera);
		CrossHair->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
		CrossHair->SetVisibility(false,true);
		//CrossHair->CreateChildActor();
	}

	static ConstructorHelpers::FClassFinder<AActor> CockPitBP(TEXT("/Game/LOTAssets/CockPit/BP/CockpitBP.CockpitBP_C"));
	CockPit = CreateDefaultSubobject<UChildActorComponent>("CockPit");
	if (CockPitBP.Class != NULL)
	{
		CockPit->SetChildActorClass(CockPitBP.Class);
		CockPit->SetupAttachment(RootComponent);
	}


	Acceleration = 300.f;
	DecreaseAccel = 150.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = -4000.f;
	CurrentForwardSpeed = 0.f;
	CurrentUpwardSpeed = 0.f;
	bHasInputUpward = false;
	bHasInputForward = false;
	bIsDetectMode = false;
	bIsPullMode = false;
	MoveAP = 2.f;
	FloatingAnim = 0.f;

}


void ALOTMultiDrone::BeginPlay()
{
	Super::BeginPlay();
	CockPit->SetVisibility(false, true);
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

}


void ALOTMultiDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	FVector forwardvector = this->GetActorForwardVector() * CurrentForwardSpeed;
	const FVector LocalMove = FVector(forwardvector.X * DeltaTime * 300, forwardvector.Y * DeltaTime * 300,  CurrentUpwardSpeed * DeltaTime * 300);
	
	//FVector physicsVector = FVector(bHasInputForward, 0, 0);//bHasInputUpward, 0);
	//Camera->GetComponentRotation().Yaw;
	
	CollisionComp->SetAllPhysicsLinearVelocity(LocalMove);//(GetVelocity() + physicsVector * CurrentForwardSpeed * DeltaTime));
	//AddMovementInput(LocalMove, CurrentForwardSpeed * DeltaTime * 100.f);

	
	//Addrelativel
	//AddActorWorldOffset(LocalMove, true);
	//		AddActorLocalOffset(LocalMove, true);

	//AddActorWorldOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);
	//AddActorWorldRotation(DeltaRotation);
	Super::Tick(DeltaTime);

	SetAnim();

	if (bIsPullMode)
		PullActor(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("������ �ӵ�= %f,�չ��� �ӵ� = %f"), CurrentUpwardSpeed, CurrentForwardSpeed));
}


void ALOTMultiDrone::SetupPlayerInputComponent(UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	
	InputComponent->BindAxis("Upward", this, &ALOTMultiDrone::MoveUpwardInput);
	InputComponent->BindAxis("Forward", this, &ALOTMultiDrone::MoveForwardInput);
	InputComponent->BindAxis("Right", this, &ALOTMultiDrone::MoveRightInput);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTMultiDrone::DetectMode);
	InputComponent->BindAction("SetTarget", IE_Pressed, this, &ALOTMultiDrone::RightClickPress);
	InputComponent->BindAction("SetTarget", IE_Released, this, &ALOTMultiDrone::RightClickRelease);

	InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ALOTMultiDrone::ChangePawn);


}

void ALOTMultiDrone::RightClickPress()
{
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameModeTest->MyPlayer.DroneMoveable) {
		if (bIsDetectMode == false)
		{
			ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
			GameInstance->SendActivateBeam();
			bIsPullMode = true;
			BeamCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			BeamCollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
			BeamCollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);
			SetBeamVisible(true);

		}
		else {
			SetTarget();
			BeamCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ALOTMultiDrone::SetBeamVisible(bool bBeam)
{
	if (bBeam)
	{
		BeamMesh->SetVisibility(true);
		
	}
	else
	{
		BeamMesh->SetVisibility(false);
	}
}

void ALOTMultiDrone::RightClickRelease()
{
	if (bIsPullMode)
	{
		bIsPullMode = false;
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
		GameInstance->SendDeactivateBeam();
	}
	
	SetBeamVisible(false);
	

}

void ALOTMultiDrone::SetTarget()
{
		AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		//ClearBeam();
		//Ÿ�ټ��� ������ Ÿ���� ���� �迭
		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
		//����ŬŸ�Ը� üũ�ϵ��� �Ѵ�.
		TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
		//���� ���带 �����´�.
		UWorld* const World = GetWorld();
		//������ ������
		FVector StartTrace = CrossHair->K2_GetComponentLocation(); //+ DetectCamera->GetForwardVector() * 500;
		//������ ����
		FVector EndTrace = StartTrace + CrossHair->GetForwardVector() * 1000000000;
		//����� ���� ����ü����
		FHitResult OutHit;
		//�������� �������� ���� ���� ����Ŭ ���Ͱ� �ִٸ�
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartTrace, EndTrace, 10.f, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, OutHit, true))
		{
			OutHit.GetActor()->GetRootPrimitiveComponent()->SetRenderCustomDepth(true);
			ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
			if (OutHit.GetActor() != GameModeTest->MyPlayer.Tank)
			{
				if (GameModeTest->MyPlayer.TargetActor == NULL) {
					GameModeTest->MyPlayer.TargetActor = OutHit.GetActor();
					GameModeTest->MyPlayer.TargetActor->GetRootPrimitiveComponent()->SetRenderCustomDepth(true);
					GameInstance->SendTargeting(true);
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� ���� ����")));
					UGameplayStatics::SpawnSound2D(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/TargetLock_Cue.TargetLock_Cue")));
				}
				else {
					GameModeTest->MyPlayer.TargetActor->GetRootPrimitiveComponent()->SetRenderCustomDepth(false);
					GameModeTest->MyPlayer.TargetActor = NULL;
					GameInstance->SendTargeting(false);
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� ���� ����")));
					UGameplayStatics::SpawnSound2D(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/TargetLock_Cue.TargetLock_Cue")));
				}
			}
		}
	

}

void ALOTMultiDrone::MoveForwardInput(float Val)
{
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("val��= %ff"), Val));
	//Val�� 0.f�� �ٻ�ġ�� �����ٸ� true,�ƴϸ� false�� ��ȯ.
	bHasInputForward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;

	//Ű �Է��� �ߴٸ�
	if (bHasInputForward )//&& GameModeTest->bIsMyTurn && GameModeTest->MyPlayer.Moveable && !GameModeTest->MyPlayer.Dead && GameModeTest->MyPlayer.DroneMoveable)
	{
		CurrentAcc = Val * Acceleration;
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
		//GameModeTest->MyPlayer.AP -= MoveAP;
		//if (GameModeTest->MyPlayer.AP <= 0) 
			//GameModeTest->MyPlayer.AP = 0;
		
	}
	//�������°� �ƴ϶��
	else if (CurrentForwardSpeed != 0.f)
	{
		if (CurrentForwardSpeed > 0)
		{
			CurrentAcc = -1.f * DecreaseAccel;
			//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� %f"), DecreaseAccel));
		}
		else if (CurrentForwardSpeed < 0)
		{
			CurrentAcc = DecreaseAccel;
			//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� %f"), DecreaseAccel));
		}
		
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		float TempClamp = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
		CurrentForwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f, 500.f) ? 0.f : TempClamp;
	}



}

void ALOTMultiDrone::MoveUpwardInput(float Val)
{

	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	
	bHasInputUpward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;


	if (bHasInputUpward )//&& GameModeTest->bIsMyTurn && GameModeTest->MyPlayer.Moveable && !GameModeTest->MyPlayer.Dead && GameModeTest->MyPlayer.DroneMoveable)
	{
		CurrentAcc = Val*  Acceleration;
		float NewUpwardSpeed = CurrentUpwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentUpwardSpeed = FMath::Clamp(NewUpwardSpeed, MinSpeed, MaxSpeed);
		//GameModeTest->MyPlayer.AP -= MoveAP;
		//if (GameModeTest->MyPlayer.AP <= 0) 
		//	GameModeTest->MyPlayer.AP = 0;
		
		

	}

	else if (CurrentUpwardSpeed != 0.f)
	{
		if (CurrentUpwardSpeed > 0)
		{
			CurrentAcc = -1.f * DecreaseAccel;
		}
		else if (CurrentUpwardSpeed < 0)
		{
			CurrentAcc = DecreaseAccel;
		}
		float NewUpwardSpeed = CurrentUpwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		float TempClamp = FMath::Clamp(NewUpwardSpeed, MinSpeed, MaxSpeed);
		CurrentUpwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f, 500.f) ? 0.f : TempClamp;


	}




}

void ALOTMultiDrone::DrawBeam(FVector StartLocation, FVector EndLocation)
{
	Beam = NewObject<UParticleSystemComponent>(this);
	Beam->RegisterComponent();
	Beam->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Particles/PT_ArcingAim.PT_ArcingAim")));

	Beam->SetBeamSourcePoint(0, StartLocation, 0);
	Beam->SetBeamTargetPoint(0, EndLocation, 0);



}

void ALOTMultiDrone::ClearBeam()
{
	Beam->DestroyComponent();
}

void ALOTMultiDrone::MoveRightInput(float Val)
{
	

	float TargetYawSpeed = (Val * TurnSpeed);


	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);


	const bool bIsTurning = FMath::Abs(Val) > 0.2f;


	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);


	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	CurrentRollSpeed = 0.f;

}

void ALOTMultiDrone::SetAnim()
{
	FloatingAnim  += 0.05f;
	BabylonMesh20->AddLocalRotation(FRotator(0.f, 0.f, CurrentYawSpeed / TurnSpeed));
	BabylonMesh21->AddLocalRotation(FRotator(0.f, 0.f, 1.0f));
	BabylonMesh->AddLocalOffset(FVector(0.f, 0.f, UKismetMathLibrary::Sin(FloatingAnim)*2.0f));
}

void ALOTMultiDrone::DetectMode()
{
	if (bIsDetectMode == false)
	{
		bIsDetectMode = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ž�����!!!"));
		Camera->Deactivate();
		//UI->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		DetectCamera->Activate();
		//UI->AttachToComponent(DetectCamera, FAttachmentTransformRules::KeepRelativeTransform);
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		BabylonMesh->SetVisibility(false, true);
		CrossHair->SetVisibility(true, true);
		CockPit->SetVisibility(false, true);
		//SetUI(false);

	}
	else
	{
		bIsDetectMode = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("��Ž�����!!!"));
		DetectCamera->Deactivate();
		//UI->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		Camera->Activate();
		//UI->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		BabylonMesh->SetVisibility(false, true);
		CrossHair->SetVisibility(false, true);
		CockPit->SetVisibility(true, true);
		//SetUI(true);

	}
}

void ALOTMultiDrone::ChangePawn()
{
	//APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	////ULOTGameInstance* const TestInstance = Cast<ULOTGameInstance>(getgamemode);
	//Test->Possess(GameModeTest->MyPlayer.Tank);

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Test->SetViewTargetWithBlend(GameModeTest->MyPlayer.Tank, 1.0f, VTBlend_EaseInOut, 10.f, true);
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = "PossessCall";
	LatentActionInfo.UUID = 123;
	LatentActionInfo.Linkage = 0;

	UKismetSystemLibrary::Delay(GetWorld(), 1.f, LatentActionInfo);

}

void ALOTMultiDrone::PossessCall()
{
	APlayerController* const Test = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AMultiGameMode* const GameModeTest = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	SetUI(false);
	GameModeTest->MyPlayer.Tank->SetUI(true);
	Test->Possess(GameModeTest->MyPlayer.Tank);
	UGameplayStatics::SpawnSound2D(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Engine/VREditor/Sounds/VR_teleport_Cue.VR_teleport_Cue")));
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("Possess call!!!")));
}

void ALOTMultiDrone::SetUI(bool bIsPlayer)
{
	if (!bIsPlayer)
	{
		
		CockPit->SetVisibility(false, true);
		BabylonMesh->SetVisibility(true, true);
		//Camera->Deactivate();
		//DetectCamera->Activate();
		CrossHair->SetVisibility(false, true);
	}
	else {
		CockPit->SetVisibility(true, true);
		BabylonMesh->SetVisibility(false, true);
		DetectCamera->Deactivate();
		Camera->Activate();
		CrossHair->SetVisibility(false, true);

	}
}

void  ALOTMultiDrone::PullActor(float time)
{
	
	TArray<AActor*> OverlapActors;

	BeamCollisionComp->GetOverlappingActors(OverlapActors, nullptr);

	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("���� ����%d"), OverlapActors.Num()));

	for (auto& Actors : OverlapActors)
	{
		if (ALOTMultiPlayer* Test = Cast<ALOTMultiPlayer>(Actors))
		{

			FVector DirVector = GetActorLocation() - Test->GetActorLocation();

			FVector NomarizeVector = UKismetMathLibrary::Normal(DirVector);

			Test->GetMesh()->SetPhysicsLinearVelocity(UKismetMathLibrary::VInterpTo(Test->GetVelocity(),
				(NomarizeVector*FVector(1.f, 1.f, 0.5f))*1900.f, time, 5.f));
			FRotator PawnRtator = Test->GetActorRotation();

			Test->SetActorRelativeRotation(UKismetMathLibrary::RInterpTo(PawnRtator, FRotator(0.f, PawnRtator.Pitch, PawnRtator.Yaw), time, 0.5f)
				, false, nullptr, ETeleportType::TeleportPhysics);


		}

	}

}

void ALOTMultiDrone::SetEmp(bool EmpState)
{
	if (EmpState == true)
	{
		CurrentForwardSpeed = 0.f;
		CurrentUpwardSpeed = 0.f;
		CurrentPitchSpeed = 0.f;
		CurrentYawSpeed = 0.f;
		CurrentRollSpeed = 0.f;
		UWorld* const World = GetWorld();
		if (World) {
			if (!EmpEffect) {
				EmpEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/ProjectilesPack/Materials/Common/LightningParticle.LightningParticle'")),
					GetActorLocation(), GetActorRotation(), true);
				EmpSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/EmpShock_Cue.EmpShock_Cue")),
					GetActorLocation(), GetActorRotation());
			}
			/*World->SpawnActor<AActor>(LoadObject<AActor>(nullptr, TEXT("Game/Blueprints/EmpEffect.EmpEffect_C"))->GetClass(),
				GetActorLocation(), GetActorRotation());*/
		}
	}
	else
	{
		if (EmpEffect)
		{
			
			EmpEffect->DestroyComponent();
			EmpSound->DestroyComponent();
		}
	}
}