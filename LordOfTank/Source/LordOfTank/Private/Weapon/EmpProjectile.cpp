// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Kismet/KismetMathLibrary.h"
#include "EmpProjectile.h"




AEmpProjectile::AEmpProjectile()
{

	CollisionComp->OnComponentHit.AddDynamic(this, &AEmpProjectile::OnHit);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/LOTAssets/TankAssets/Meshes/CommonAmmo.CommonAmmo'"));


	//static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'"));
	//TrailParticle->SetTemplate(TrailParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/MicrodestructorMissile/P_Microdestructor_Fire.P_Microdestructor_Fire'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/LOTAssets/TankAssets/Audio/Flyby_Cue.Flyby_Cue"));
	FlySoundComponent->SetSound(SoundCue.Object);

	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);
	FlareParticle->SetTemplate(FlareParticleAsset.Object);
	FlareParticle->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	ExplosionParticle = ExplosionParticleAsset.Object;


	RadialRadius = 1000.f; //���� �ݰ�
	ImpulseStrength = 1000000.f;
	ProjectileDamage = 50.f;

	AddCollisionChannelToAffect(ECC_MAX);
	//AddCollisionChannelToAffect(ECC_Pawn);
	//AddCollisionChannelToAffect(ECC_PhysicsBody);
	//AddCollisionChannelToAffect(ECC_Vehicle);
	//AddCollisionChannelToAffect(ECC_Destructible);
	UpdateCollisionObjectQueryParams();


}

void AEmpProjectile::BeginPlay()
{
	Super::BeginPlay();

	FlySoundComponent->Play();

	//MakeInvisability();


}

void AEmpProjectile::AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType)
{
	ObjectTypesToAffect.AddUnique(ObjectType);
	UpdateCollisionObjectQueryParams();
}

void AEmpProjectile::AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel)
{
	EObjectTypeQuery ObjectType = UEngineTypes::ConvertToObjectType(CollisionChannel);
	if (ObjectType != ObjectTypeQuery_MAX)
	{
		AddObjectTypeToAffect(ObjectType);
	}
}

void AEmpProjectile::UpdateCollisionObjectQueryParams()
{
	CollisionObjectQueryParams = FCollisionObjectQueryParams(ObjectTypesToAffect);
}


void AEmpProjectile::FireImpulse()
{
	ERadialImpulseFalloff Falloff;
	Falloff = RIF_Linear;
	const FVector Origin = GetActorLocation();


	static FName FireImpulseOverlapName = FName(TEXT("FireImpulseOverlap"));
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(FireImpulseOverlapName, false);
	Params.bTraceAsyncScene = true;



	Params.AddIgnoredActor(GetOwner());
	int count = 0;

	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(RadialRadius), Params);

	TArray<AActor*, TInlineAllocator<1>> AffectedActors;
	

	for (FOverlapResult& OverlapResult : Overlaps)
	{
	
		if (AActor* InsideActor = OverlapResult.Actor.Get())
		{

			AffectedActors.AddUnique(InsideActor);

		}
	}



	for (AActor* InsideActor : AffectedActors)
	{

		AMultiGameMode* const GameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
		ALordOfTankGameModeBase* const SingleMode = Cast<ALordOfTankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		ATrainingMode* const TrainingGameMode = Cast<ATrainingMode>(UGameplayStatics::GetGameMode(GetWorld()));
		//��Ƽ����
		if (GameMode)
		{

			if (ALOTMultiDrone* const Test = Cast<ALOTMultiDrone>(InsideActor)) {
				//�� �ڽſ��� �¾��� ��� 
				if ((bIsFireEnemy == false && Test == ParentDrone))
				{
					GameInstance->SendTankHit(0.f, PROJECTILE_EMP); 
				}
				//������ �¾��� ���
				else if ((bIsFireEnemy == true && Test != ParentDrone))
				{
					GameInstance->SendTankHit(0.f, PROJECTILE_EMP);
				}
			}
		}
		//�̱۰���
		else if (SingleMode == UGameplayStatics::GetGameMode(GetWorld()) || TrainingGameMode) {

			if (ALOTDrone* const Test = Cast<ALOTDrone>(InsideActor)) {
				//�� �ڽſ��� �¾��� ��� 
				if ((bIsFireEnemy == false && Test == ParentDrone))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;

					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					
				}
				//������ �¾��� ���
				else if ((Test != ParentDrone))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;
					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���̽�� �������� %f!!"), ProjectileDamage*DamageRatio));
					//GameInstance->SendTankHit(ProjectileDamage*DamageRatio);
					
				}
			}
		}


	}

}


void AEmpProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


	ALordOfTankGameModeBase* const SingleMode = Cast<ALordOfTankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ATrainingMode* const TrainingGameMode = Cast<ATrainingMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ALOTPlayer* const Test = Cast<ALOTPlayer>(ParentTank);
	//���Ϳ� �¾��� �����
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (SingleMode && Test->bIsTestShot && Test->GetisAI() && OtherActor == Test->Player) {
			Test->bIsTestShot = false;
			Test->RightShot = true;
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("�׽�Ʈ ��")));
		}
		else {
			if (SingleMode && !Test->bIsTestShot)
				Test->ChangeTurn();
		}
	}

	if (TrainingGameMode)
		Test->ChangeTurn();


	//���� if�� �ȿ� ������ ���Ϳ� �浹�Ҷ��� �������༭ �ֺ��� �������� �� ��������� ����
	FireImpulse();



	AMultiGameMode* const GameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//��Ƽ����
	if (GameMode) {
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
		ALOTMultiPlayer* MultiParent = Cast<ALOTMultiPlayer>(ParentTank);
		MultiParent->SetbIsShoot(false);
		GameInstance->SendExplosion();
	}


	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), true)->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/MicrodestructorMissile/P_Microdestructor_Swarm.P_Microdestructor_Swarm'")),
		GetActorLocation(), GetActorRotation(), true)->SetRelativeScale3D(FVector(15.0f, 15.0f, 15.0f));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/Explosion_Cue.Explosion_Cue")),
		GetActorLocation(), GetActorRotation());
	Destroy();
}