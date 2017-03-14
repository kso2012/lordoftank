// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "LOTDrone.h"


// Sets default values
ALOTDrone::ALOTDrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/LOTAssets/DroneAssets/StaticMesh.StaticMesh"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

											 // Set handling parameters
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

}

// Called when the game starts or when spawned
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
	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);


	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

	// Call any parent class Tick implementation
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

}


void ALOTDrone::MoveForwardInput(float Val)
{
	// Is there no input?
	bHasInputForward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = 0.f;
	// If input is not held down, reduce speed

	if (bHasInputForward)
	{
		CurrentAcc = Val * Acceleration;
		bAcceleratedForward = (Val > 0) ? true : false;
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
	}

	else if (CurrentForwardSpeed != 0.f)
	{
		if (bAcceleratedForward)
		{
			CurrentAcc = -1.f * Acceleration;
		}
		else
		{
			CurrentAcc =  Acceleration;
		}
		float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		float TempClamp = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
		CurrentForwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f, 50.f) ? 0.f : TempClamp;
	}


}

void ALOTDrone::MoveUpwardInput(float Val)
{

	// Is there no input?
	bHasInputUpward = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc=0.f;
	// If input is not held down, reduce speed

	if (bHasInputUpward)
	{

		
		CurrentAcc = Val * Acceleration;
		bAcceleratedUpward = (Val > 0) ? true : false;
		// Calculate new speed
		float NewUpwardSpeed = CurrentUpwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Blue, FString::Printf(TEXT("%f"), NewUpwardSpeed));
		// Clamp between MinSpeed and MaxSpeed
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
		CurrentUpwardSpeed = FMath::IsNearlyEqual(TempClamp, 0.f,50.f) ? 0.f : TempClamp;


	}

	


}

void ALOTDrone::MoveRightInput(float Val)
{

	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);
	
	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	CurrentRollSpeed = 0.f;

}
