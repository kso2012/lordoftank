// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTDrone.h"



ALOTDrone::ALOTDrone()
{

	PrimaryActorTick.bCanEverTick = true;


	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/LOTAssets/DroneAssets/StaticMesh.StaticMesh"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;


	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 160.0f; 
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; 

	DetectCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	DetectCamera->bUsePawnControlRotation = false;
	DetectCamera->FieldOfView = 90.f;
	DetectCamera->SetupAttachment(RootComponent);
	DetectCamera->Deactivate();

										
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = -4000.f;
	CurrentForwardSpeed = 0.f;
	CurrentUpwardSpeed = 0.f;
	bAcceleratedUpward = false;
	bAcceleratedForward = false;
	bHasInputUpward = false;
	bHasInputForward = false;
	bIsDetectMode = false;
	
}


void ALOTDrone::BeginPlay()
{
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	
}

// Called every frame
void ALOTDrone::Tick(float DeltaTime)
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


	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("위방향 속도= %f,앞방향 속도 = %f"), CurrentUpwardSpeed, CurrentForwardSpeed));
}

// Called to bind functionality to input
void ALOTDrone::SetupPlayerInputComponent(UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	// Bind our control axis' to callback functions
	//InputComponent->BindAxis("Thrust", this, &ALOTDrone::ThrustInput);
	InputComponent->BindAxis("Upward", this, &ALOTDrone::MoveUpwardInput);
    InputComponent->BindAxis("Forward", this, &ALOTDrone::MoveForwardInput);
	InputComponent->BindAxis("Right", this, &ALOTDrone::MoveRightInput);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTDrone::DetectMode);
	InputComponent->BindAction("SetTarget", IE_Pressed, this, &ALOTDrone::SetTarget);
	

}

void ALOTDrone::SetTarget()
{
	//타겟설정 가능한 타입을 넣을 배열
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	//비히클타입만 체크하도록 한다.
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	//현재 월드를 가져온다.
	UWorld* const World = GetWorld();
	//벡터의 시작점
	FVector StartTrace = DetectCamera->K2_GetComponentLocation();
	//벡터의 끝점
	FVector EndTrace = StartTrace + DetectCamera->GetForwardVector() * 500000;
	//결과를 담을 구조체변수
	FHitResult OutHit;
	//시작점과 끝점간에 빛을 쏴서 비히클 액터가 있다면
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartTrace, EndTrace, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHit, true)) {
		HomingTarget = OutHit.GetActor();
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, "Target Name = " + HomingTarget->GetName());
	}
}

void ALOTDrone::MoveForwardInput(float Val)
{
	//Val이 0.f와 근사치에 가깝다면 true,아니면 false를 반환.
	bHasInputForward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;

	//키 입력을 했다면
	if (bHasInputForward)
	{
		CurrentAcc = Val * Acceleration;
		bAcceleratedForward = (Val > 0) ? true : false;
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
	}
	//정지상태가 아니라면
	else if (CurrentForwardSpeed != 0.f)
	{
		//전방으로 가속하고 있었다면 
		if (bAcceleratedForward)
		{
			CurrentAcc = -1.f * Acceleration;
		}
		//후방으로 가속하고 있었다면
		else
		{
			CurrentAcc =  Acceleration;
		}
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		float TempClamp = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
		CurrentForwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f, 500.f) ? 0.f : TempClamp;
	}


}

void ALOTDrone::MoveUpwardInput(float Val)
{


	bHasInputUpward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc=0.f;


	if (bHasInputUpward)
	{

		
		CurrentAcc = Val * Acceleration;
		bAcceleratedUpward = (Val > 0) ? true : false;

		float NewUpwardSpeed = CurrentUpwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Blue, FString::Printf(TEXT("%f"), NewUpwardSpeed));

		CurrentUpwardSpeed = FMath::Clamp(NewUpwardSpeed, MinSpeed, MaxSpeed);
		
	}

	else if (CurrentUpwardSpeed != 0.f)
	{

		if (bAcceleratedUpward)
		{
			CurrentAcc = -1.f * Acceleration;

		}
		else
		{

			CurrentAcc = Acceleration;

		}
		float NewUpwardSpeed = CurrentUpwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		float TempClamp = FMath::Clamp(NewUpwardSpeed, MinSpeed, MaxSpeed);
		CurrentUpwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f,500.f) ? 0.f : TempClamp;


	}

	


}

void ALOTDrone::MoveRightInput(float Val)
{


	float TargetYawSpeed = (Val * TurnSpeed);
	

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);


	const bool bIsTurning = FMath::Abs(Val) > 0.2f;


	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);


	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	CurrentRollSpeed = 0.f;

}

void ALOTDrone::DetectMode()
{
	if (bIsDetectMode == false)
	{
		bIsDetectMode = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("탐색모드!!!"));
		Camera->Deactivate();
		DetectCamera->Activate();
		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		PlaneMesh->SetVisibility(false, false);

	}
	else
	{
		bIsDetectMode = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("노탐색모드!!!"));
		Camera->Activate();
		DetectCamera->Deactivate();

		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		PlaneMesh->SetVisibility(true, false);
		

	}
}