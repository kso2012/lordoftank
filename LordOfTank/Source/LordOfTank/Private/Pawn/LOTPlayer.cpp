// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Vehicle/FrontWheel.h"
#include "Vehicle/RearWheel.h"
#include "Weapon/Projectile.h"
#include "Weapon/CommonProjectile.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/HomingProjectile.h"
#include "Effects/TankCameraShake.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "LOTDrone.h"
#include "LOTPlayer.h"




ALOTPlayer::ALOTPlayer()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	PrimaryActorTick.bCanEverTick = true;
	//스켈레톤컴포넌트에 메쉬 적용.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySkeletalMesh(TEXT("/Game/LOTAssets/TankAssets/LOTBody.LOTBody"));
	GetMesh()->SetSkeletalMesh(BodySkeletalMesh.Object);
	//스켈레톤컴포넌트에 애니메이션 적용.
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/LOTAssets/TankAssets/LOTPlaytankAnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	GetMesh()->OnComponentHit.AddDynamic(this, &ALOTPlayer::OnHit);
	//GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ALOTPlayer::OnOverlapBegin);
	
	//터렛컴포넌트에 메쉬 적용.
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretStaticMesh(TEXT("/Game/LOTAssets/TankAssets/Meshes/LBX1Turret_SM"));
	TurretMesh->SetStaticMesh(TurretStaticMesh.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> TurretMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	TurretMesh->SetMaterial(0,TurretMaterial.Object);
	TurretMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelStaticMesh(TEXT("/Game/LOTAssets/TankAssets/Meshes/LBX1Barrel_SM"));
	BarrelMesh->SetStaticMesh(BarrelStaticMesh.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> BarrelMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	BarrelMesh->SetMaterial(0, BarrelMaterial.Object);
	BarrelMesh->AttachToComponent(TurretMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Turret_BR"));
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	//총구에 씬컴포넌트 부착.
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->AttachToComponent(BarrelMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	

	
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

	

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;


	// Create camera component
	MoveModeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	MoveModeCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	MoveModeCamera->bUsePawnControlRotation = false;
	MoveModeCamera->FieldOfView = 90.f;

	FireModeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	FireModeCamera->bUsePawnControlRotation = false;
	FireModeCamera->FieldOfView = 90.f;
	FireModeCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	FireModeCamera->Deactivate();



	bIsFireMode = false;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;


	
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
	InputComponent->BindAction("Fire", IE_Pressed, this, &ALOTPlayer::Fire);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &ALOTPlayer::FireMode);
	InputComponent->BindAction("One", IE_Pressed, this, &ALOTPlayer::One);
	InputComponent->BindAction("Two", IE_Pressed, this, &ALOTPlayer::Two);


}
void ALOTPlayer::One()
{
	
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));

	UWorld* const World = GetWorld();
	FVector StartTrace = MuzzleLocation->K2_GetComponentLocation();
	FVector EndTrace = MuzzleLocation->K2_GetComponentLocation() + MuzzleLocation->GetForwardVector() * 5000;

	FHitResult OutHit;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartTrace, EndTrace, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHit, true)) {
		HomingTarget = OutHit.GetActor();
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, "Target Name = " + HomingTarget->GetName());
	}

}
void ALOTPlayer::Two()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Two!!!"));
	CurrentProjectile =ProjectileInventory[1];
}

void ALOTPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFireMode)
	{
		TurretMesh->SetRelativeRotation(FRotator(0.0f, FireModeCamera->RelativeRotation.Yaw, 0.0f));
		BarrelMesh->SetRelativeRotation(FRotator(FireModeCamera->RelativeRotation.Pitch, 0.0f, 0.0f));
	}
}

void ALOTPlayer::FireMode()
{
	if (bIsFireMode == false)
	{
		bIsFireMode = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("포격모드!!!"));
		MoveModeCamera->Deactivate();
		FireModeCamera->Activate();
		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		TurretMesh->SetVisibility(false, false);
		GetMesh()->SetVisibility(false, false);
		BarrelMesh->SetVisibility(false, false);
	}
	else
	{
		bIsFireMode = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("노포격모드!!!"));
		MoveModeCamera->Activate();
		FireModeCamera->Deactivate();

		//1번째 인자false->hide,2번째 인자 false->자식 컴포넌트도 영향을 미친다.
		TurretMesh->SetVisibility(true, false);
		GetMesh()->SetVisibility(true, false);
		BarrelMesh->SetVisibility(true, false);
	
	}
}




void ALOTPlayer::Fire()
{
	
	if (CurrentProjectile != NULL && bIsFireMode)
	{
		const FRotator SpawnRotation = GetActorRotation()+ FireModeCamera->RelativeRotation;//
		
		const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) ;

		UWorld* const World = GetWorld();
		if (World != NULL)
		{

			AActor* TempActor = World->SpawnActor<AActor>(CurrentProjectile, SpawnLocation, SpawnRotation);
			//World->SpawnActor<AProjectile>(CurrentProjectile, SpawnLocation, SpawnRotation)->SetHomingTarget(HomingTarget);
			//// spawn the pickup
			//APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			//World->SpawnActor<ALOTDrone>(ALOTDrone::StaticClass(), SpawnLocation+FVector(0.0f,0.0f,1000.f), SpawnRotation);
			
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), UTankCameraShake::StaticClass(), GetActorLocation(), 0.f, 500.f, false);
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(TempActor,1.0f,VTBlend_Linear,0.0f,true);

		}
	}
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("배열길이 %d"), ProjectileInventory.Num()));

}

void ALOTPlayer::Turn(float Val)
{
	
}



void ALOTPlayer::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

/** Handle pressing forwards */
void ALOTPlayer::MoveForward(float Val)
{
	if(!bIsFireMode)
		GetVehicleMovementComponent()->SetThrottleInput(Val);

}

/** Handle pressing right */
void ALOTPlayer::MoveRight(float Val)
{
	if(!bIsFireMode)
		GetVehicleMovementComponent()->SetSteeringInput(Val);

}

void ALOTPlayer::SetDefaultInvetory()
{
	if (ProjectileInventory.Num() == 0)
	{
		ProjectileInventory.AddUnique(ACommonProjectile::StaticClass());
		ProjectileInventory.AddUnique(AArmorPiercingProjectile::StaticClass());
		ProjectileInventory.AddUnique(AHomingProjectile::StaticClass());
		CurrentProjectile = ProjectileInventory[1];

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



void ALOTPlayer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		;
	}
}

