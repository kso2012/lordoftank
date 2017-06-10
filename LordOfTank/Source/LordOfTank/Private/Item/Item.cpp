// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Components/BoxComponent.h"
#include "Item.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySkeletalMesh(TEXT("/Game/LOTAssets/TankAssets/LOTBody.LOTBody"));
	//GetMesh()->SetSkeletalMesh(BodySkeletalMesh.Object);
	//스켈레톤컴포넌트에 애니메이션 적용.
	//static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/LOTAssets/TankAssets/LOTPlaytankAnimBP"));
	//GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	//GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ALOTPlayer::OnOverlapBegin);

	//터렛컴포넌트에 메쉬 적용.
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxStaticMesh(TEXT("/Game/LOTAssets/ItemAssets/Box/WoodenBox02"));
	BoxMesh->SetStaticMesh(BoxStaticMesh.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> TurretMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	//TurretMesh->SetMaterial(0,TurretMaterial.Object);
	//TurretMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	BoxMesh->SetupAttachment(RootComponent, TEXT("Box"));
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxMesh->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	BoxMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	BoxMesh->SetWorldScale3D(FVector(500, 500, 500));

	ParachuteMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ParachuteMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ParachuteStaticMesh(TEXT("/Game/LOTAssets/ItemAssets/Parachute/Parachute"));
	ParachuteMesh->SetSkeletalMesh(ParachuteStaticMesh.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> TurretMaterial(TEXT("/Game/LOTAssets/TankAssets/Materials/LBXMY_MAT"));
	//TurretMesh->SetMaterial(0,TurretMaterial.Object);
	//TurretMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Body_TR"));
	ParachuteMesh->SetupAttachment(BoxMesh, TEXT("Parachute"));
	ParachuteMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ParachuteSize = 0.f;

	ParachuteMesh->SetWorldRotation(FRotator(0, 90, 90));
	ParachuteMesh->SetWorldScale3D(FVector(ParachuteSize, ParachuteSize, ParachuteSize));
	ParachuteMesh->SetRelativeLocation(BoxMesh->RelativeLocation);
	ParachuteMesh->AddRelativeLocation(FVector(0, 0, -2));

	FallingSpeed = 0.f;

	CollBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CollBox->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	CollBox->SetupAttachment(BoxMesh, TEXT("BoxCollision")); 
	CollBox->SetCollisionObjectType(ECC_WorldStatic);
	CollBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollBox->UPrimitiveComponent::SetCollisionResponseToAllChannel(ECR_Overlap);
	CollBox->SetCollisionResponseToAllChannels(ECR_Block);
	//CollBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollBox->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);


	CollBox->bGenerateOverlapEvents = true;
	CollBox->OnComponentBeginOverlap.__Internal_AddDynamic(this, &AItem::CheckFalling, FName(TEXT("CheckFalling")));
	CollBox->SetVisibility(true);
	
	BoxMesh->SetSimulatePhysics(false);
	BoxMesh->SetEnableGravity(false);

	bIsFalling = true;
	bUseGravity = false;

	Gravity = 9.8f;
	bShakePlus = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector BoxLocation = BoxMesh->GetComponentLocation();
	

	SetFallingSpeed();

	if (!bUseGravity) {
		if (ParachuteSize < 10.f) {
			ParachuteSize += 0.1f;
			ParachuteMesh->SetWorldScale3D(FVector(ParachuteSize, ParachuteSize, ParachuteSize));
		}
		CheckLocation();
		BoxLocation.Z -= FallingSpeed * DeltaTime;

		ShakeBox();
	}
	else {
		if (ParachuteSize > 0.f) {
			ParachuteSize -= 0.5f;
			ParachuteMesh->SetWorldScale3D(FVector(ParachuteSize, ParachuteSize, ParachuteSize));
			if(ParachuteSize < 5.f)
				BoxMesh->SetEnableGravity(true);
		}
		BoxLocation.Z -= (FallingSpeed + Gravity) * DeltaTime;
		Gravity += 9.8f;
		if (ParachuteSize <= 0.f) {
			FallingSpeed = 0.f;
			Gravity = 0.f;
			ParachuteMesh->DestroyComponent();
		}
	}


	BoxMesh->SetWorldLocation(BoxLocation);
}

void AItem::SetFallingSpeed() {
	FallingSpeed += 1.f;
}

void AItem::CheckFalling(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsFalling = false;
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), bIsFalling));
	/*
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		Destroy();
	}*/
}

void AItem::CheckLocation() {

	//타겟설정 가능한 타입을 넣을 배열
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	//비히클타입만 체크하도록 한다.
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	//현재 월드를 가져온다.
	UWorld* const World = GetWorld();
	//벡터의 시작점
	FVector StartTrace = BoxMesh->GetComponentLocation(); //+ DetectCamera->GetForwardVector() * 500;
	StartTrace.Z -= 500.f;
															   //벡터의 끝점
	FVector EndTrace = StartTrace;
	EndTrace.Z -= 5000.f;

	//결과를 담을 구조체변수
	FHitResult OutHit;
	//시작점과 끝점간에 빛을 쏴서 비히클 액터가 있다면
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartTrace, EndTrace, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::None, OutHit, false))
	{
		bUseGravity = true;
		BoxMesh->SetSimulatePhysics(true);
	}
}

void AItem::ShakeBox() {
	if (bShakePlus) {
		BoxMesh->AddRelativeRotation(FRotator(0, 0, 0.1f));
		if (BoxMesh->RelativeRotation.Roll > 10.f)
			bShakePlus = false;
	}
	else {
		BoxMesh->AddRelativeRotation(FRotator(0, 0, -0.1f));
		if (BoxMesh->RelativeRotation.Roll < -10.f)
			bShakePlus = true;
	}
}