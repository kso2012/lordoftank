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


	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("������ �ӵ�= %f,�չ��� �ӵ� = %f"), CurrentUpwardSpeed, CurrentForwardSpeed));
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
	//Ÿ�ټ��� ������ Ÿ���� ���� �迭
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	//����ŬŸ�Ը� üũ�ϵ��� �Ѵ�.
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	//���� ���带 �����´�.
	UWorld* const World = GetWorld();
	//������ ������
	FVector StartTrace = DetectCamera->K2_GetComponentLocation();
	//������ ����
	FVector EndTrace = StartTrace + DetectCamera->GetForwardVector() * 500000;
	//����� ���� ����ü����
	FHitResult OutHit;
	//�������� �������� ���� ���� ����Ŭ ���Ͱ� �ִٸ�
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartTrace, EndTrace, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHit, true)) {
		HomingTarget = OutHit.GetActor();
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, "Target Name = " + HomingTarget->GetName());
	}
}

void ALOTDrone::MoveForwardInput(float Val)
{
	//Val�� 0.f�� �ٻ�ġ�� �����ٸ� true,�ƴϸ� false�� ��ȯ.
	bHasInputForward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;

	//Ű �Է��� �ߴٸ�
	if (bHasInputForward)
	{
		CurrentAcc = Val * Acceleration;
		bAcceleratedForward = (Val > 0) ? true : false;
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
	}
	//�������°� �ƴ϶��
	else if (CurrentForwardSpeed != 0.f)
	{
		//�������� �����ϰ� �־��ٸ� 
		if (bAcceleratedForward)
		{
			CurrentAcc = -1.f * Acceleration;
		}
		//�Ĺ����� �����ϰ� �־��ٸ�
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ž�����!!!"));
		Camera->Deactivate();
		DetectCamera->Activate();
		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		PlaneMesh->SetVisibility(false, false);

	}
	else
	{
		bIsDetectMode = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("��Ž�����!!!"));
		Camera->Activate();
		DetectCamera->Deactivate();

		//1��° ����false->hide,2��° ���� false->�ڽ� ������Ʈ�� ������ ��ģ��.
		PlaneMesh->SetVisibility(true, false);
		

	}
}