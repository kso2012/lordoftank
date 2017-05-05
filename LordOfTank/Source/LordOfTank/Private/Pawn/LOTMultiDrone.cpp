// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTMultiDrone.h"
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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BabylonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh"));
	BabylonMesh->SetStaticMesh(ConstructorStatics.BabylonMesh.Get());
	BabylonMesh->SetupAttachment(Root);

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
	//몸통 메쉬
	BabylonMesh20 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh20"));
	BabylonMesh20->SetStaticMesh(ConstructorStatics.BabylonMesh20.Get());
	BabylonMesh20->SetupAttachment(BabylonMesh);
	//날개 몸통
	BabylonMesh21 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh21"));
	BabylonMesh21->SetStaticMesh(ConstructorStatics.BabylonMesh21.Get());
	BabylonMesh21->SetupAttachment(BabylonMesh);
	//날개2
	BabylonMesh22 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh22"));
	BabylonMesh22->SetStaticMesh(ConstructorStatics.BabylonMesh22.Get());
	BabylonMesh22->SetupAttachment(BabylonMesh21);

	//날개에 붙은 뼈대1
	BabylonMesh23 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh23"));
	BabylonMesh23->SetStaticMesh(ConstructorStatics.BabylonMesh23.Get());
	BabylonMesh23->SetupAttachment(BabylonMesh22);

	//날개에 붙은 뼈대2
	BabylonMesh12 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BabylonMesh12"));
	BabylonMesh12->SetStaticMesh(ConstructorStatics.BabylonMesh12.Get());
	BabylonMesh12->SetupAttachment(BabylonMesh21);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 160.0f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(-1000.0f, 0.0f, 470.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	DetectCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	DetectCamera->bUsePawnControlRotation = false;
	DetectCamera->FieldOfView = 90.f;
	DetectCamera->SetupAttachment(RootComponent);
	DetectCamera->Deactivate();


	static ConstructorHelpers::FClassFinder<AActor> CrossHairBP(TEXT("/Game/Blueprints/crossBP.crossBP_C"));
	CrossHair = CreateDefaultSubobject<UChildActorComponent>("CrossHair");
	if (CrossHairBP.Class != NULL)
	{
		CrossHair->SetChildActorClass(CrossHairBP.Class);
		CrossHair->SetupAttachment(DetectCamera);
		CrossHair->SetVisibility(false,true);
		//CrossHair->CreateChildActor();
	}



	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = -4000.f;
	CurrentForwardSpeed = 0.f;
	CurrentUpwardSpeed = 0.f;
	bHasInputUpward = false;
	bHasInputForward = false;
	bIsDetectMode = false;

}


void ALOTMultiDrone::BeginPlay()
{
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

}

// Called every frame
void ALOTMultiDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, CurrentUpwardSpeed * DeltaTime);

	AddActorLocalOffset(LocalMove, true);



	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;


	AddActorLocalRotation(DeltaRotation);

	Super::Tick(DeltaTime);

	SetAnim();

	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("위방향 속도= %f,앞방향 속도 = %f"), CurrentUpwardSpeed, CurrentForwardSpeed));
}

// Called to bind functionality to input
void ALOTMultiDrone::SetupPlayerInputComponent(UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	// Bind our control axis' to callback functions
	//InputComponent->BindAxis("Thrust", this, &ALOTMultiDrone::ThrustInput);
	InputComponent->BindAxis("Upward", this, &ALOTMultiDrone::MoveUpwardInput);
	InputComponent->BindAxis("Forward", this, &ALOTMultiDrone::MoveForwardInput);
	InputComponent->BindAxis("Right", this, &ALOTMultiDrone::MoveRightInput);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTMultiDrone::DetectMode);
	InputComponent->BindAction("SetTarget", IE_Pressed, this, &ALOTMultiDrone::SetTarget);

	InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ALOTMultiDrone::ChangePawn);


}

void ALOTMultiDrone::SetTarget()
{
	//ClearBeam();
	//타겟설정 가능한 타입을 넣을 배열
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	//비히클타입만 체크하도록 한다.
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	//현재 월드를 가져온다.
	UWorld* const World = GetWorld();
	//벡터의 시작점
	FVector StartTrace = DetectCamera->K2_GetComponentLocation() + DetectCamera->GetForwardVector() * 500;
	//벡터의 끝점
	FVector EndTrace = StartTrace + DetectCamera->GetForwardVector() * 500000;
	//결과를 담을 구조체변수
	FHitResult OutHit;
	//시작점과 끝점간에 빛을 쏴서 비히클 액터가 있다면
	if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartTrace, EndTrace, 10.f, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, OutHit, true))
	{
		if(HomingTarget != NULL)
			HomingTarget->GetRootPrimitiveComponent()->SetRenderCustomDepth(false);
		HomingTarget = OutHit.GetActor();
		HomingTarget->GetRootPrimitiveComponent()->SetRenderCustomDepth(true);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "Target Name = " + HomingTarget->GetName());
		//DrawBeam(StartTrace, EndTrace);
	}

}

void ALOTMultiDrone::MoveForwardInput(float Val)
{
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("val값= %ff"), Val));
	//Val이 0.f와 근사치에 가깝다면 true,아니면 false를 반환.
	bHasInputForward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;

	//키 입력을 했다면
	if (bHasInputForward)
	{
		CurrentAcc = Val * Acceleration;
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
	}
	//정지상태가 아니라면
	else if (CurrentForwardSpeed != 0.f)
	{
		if (CurrentForwardSpeed > 0)
		{
			CurrentAcc = -1.f * Acceleration;
		}
		else if (CurrentForwardSpeed < 0)
		{
			CurrentAcc = Acceleration;
		}
		
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		float TempClamp = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
		CurrentForwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f, 500.f) ? 0.f : TempClamp;
	}



}

void ALOTMultiDrone::MoveUpwardInput(float Val)
{


	bHasInputUpward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;


	if (bHasInputUpward)
	{
		CurrentAcc = Val * Acceleration;
		float NewUpwardSpeed = CurrentUpwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentUpwardSpeed = FMath::Clamp(NewUpwardSpeed, MinSpeed, MaxSpeed);

	}

	else if (CurrentUpwardSpeed != 0.f)
	{
		if (CurrentUpwardSpeed > 0)
		{
			CurrentAcc = -1.f * Acceleration;
		}
		else if (CurrentUpwardSpeed < 0)
		{
			CurrentAcc = Acceleration;
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
	BabylonMesh20->AddLocalRotation(FRotator(0.f, 0.f, CurrentYawSpeed / TurnSpeed));
	BabylonMesh21->AddLocalRotation(FRotator(0.f, 0.f, 1.0f));
}

void ALOTMultiDrone::DetectMode()
{
	if (bIsDetectMode == false)
	{
		bIsDetectMode = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("탐색모드!!!"));
		Camera->Deactivate();
		DetectCamera->Activate();
		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		BabylonMesh->SetVisibility(false, true);
		CrossHair->SetVisibility(true, true);

	}
	else
	{
		bIsDetectMode = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("노탐색모드!!!"));
		Camera->Activate();
		DetectCamera->Deactivate();

		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		BabylonMesh->SetVisibility(true, true);
		CrossHair->SetVisibility(false, true);


	}
}

void ALOTMultiDrone::ChangePawn()
{
	;
}