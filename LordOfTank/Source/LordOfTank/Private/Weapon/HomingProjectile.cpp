// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Pawn/LOTPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/HomingProjectile.h"
#include "Weapon/Projectile.h"




AHomingProjectile::AHomingProjectile()
{
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AHomingProjectile::OnHit);		


	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/ProjectilesPack/Meshes/Projectiles/Rockets/SM_Rocket_04.SM_Rocket_04'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);
	AmmoMesh->SetRelativeLocation(FVector(-300.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'"));
	TrailParticle->SetTemplate(TrailParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'"));
	FlareParticle->SetTemplate(FlareParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_ExplosionWithShrapnel.P_ExplosionWithShrapnel'"));
	ExplosionParticle = ExplosionParticleAsset.Object;
	

	AddCollisionChannelToAffect(ECC_MAX);
	//AddCollisionChannelToAffect(ECC_Pawn);
	//AddCollisionChannelToAffect(ECC_PhysicsBody);
	//AddCollisionChannelToAffect(ECC_Vehicle);
	//AddCollisionChannelToAffect(ECC_Destructible);
	UpdateCollisionObjectQueryParams();

	RadialRadius = 1000.f; //���� �ݰ�
	ImpulseStrength = 1000000.f;
	ProjectileDamage = 50.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 100.f;

}

void AHomingProjectile::AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType)
{
	ObjectTypesToAffect.AddUnique(ObjectType);
	UpdateCollisionObjectQueryParams();
}

void AHomingProjectile::AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel)
{
	EObjectTypeQuery ObjectType = UEngineTypes::ConvertToObjectType(CollisionChannel);
	if (ObjectType != ObjectTypeQuery_MAX)
	{
		AddObjectTypeToAffect(ObjectType);
	}
}

void AHomingProjectile::UpdateCollisionObjectQueryParams()
{
	CollisionObjectQueryParams = FCollisionObjectQueryParams(ObjectTypesToAffect);
}


void AHomingProjectile::FireImpulse()
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

	TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
	TArray<AActor*, TInlineAllocator<1>> AffectedActors;
	AffectedComponents.Reserve(Overlaps.Num());

	for (FOverlapResult& OverlapResult : Overlaps)
	{
		if (UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
		{

			AffectedComponents.AddUnique(PrimitiveComponent);

		}

		if (AActor* InsideActor = OverlapResult.Actor.Get())
		{

			AffectedActors.AddUnique(InsideActor);

		}
	}

	for (UPrimitiveComponent* PrimitiveComponent : AffectedComponents)
	{
		if (ImpulseStrength > SMALL_NUMBER)
		{
			if (UDestructibleComponent* DestructibleComponent = Cast<UDestructibleComponent>(PrimitiveComponent))
			{
				DestructibleComponent->ApplyRadiusDamage(ImpulseStrength, Origin, RadialRadius, ImpulseStrength, Falloff == RIF_Constant);
			}
		}

		PrimitiveComponent->AddRadialImpulse(Origin, RadialRadius, ImpulseStrength, Falloff, false);
	}

	for (AActor* InsideActor : AffectedActors)
	{

		AMultiGameMode* const GameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
		ALordOfTankGameModeBase* const SingleMode = Cast<ALordOfTankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		//��Ƽ����
		if (GameMode)
		{

			if (ALOTMultiPlayer* const Test = Cast<ALOTMultiPlayer>(InsideActor)) {
				//�� �ڽſ��� �¾��� ��� 
				if ((bIsFireEnemy == false && Test == ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;

					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("������� �������� %f!!"), ProjectileDamage*DamageRatio));
					GameInstance->SendTankHit(ProjectileDamage*DamageRatio, PROJECTILE_HOMING);
				}
				//������ �¾��� ���
				else if ((bIsFireEnemy == true && Test != ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;
					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���̽�� �������� %f!!"), ProjectileDamage*DamageRatio));
					GameInstance->SendTankHit(ProjectileDamage*DamageRatio, PROJECTILE_HOMING);
				}
			}
		}
		//�̱۰���
		else if (SingleMode == UGameplayStatics::GetGameMode(GetWorld())) {

			if (ALOTPlayer* const Test = Cast<ALOTPlayer>(InsideActor)) {
				//�� �ڽſ��� �¾��� ��� 
				if ((bIsFireEnemy == false && Test == ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;

					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("������� �������� %f!!"), ProjectileDamage*DamageRatio));
					//GameInstance->SendTankHit(ProjectileDamage*DamageRatio);
				}
				//������ �¾��� ���
				else if ((bIsFireEnemy == true && Test != ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;
					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("���̽�� �������� %f!!"), ProjectileDamage*DamageRatio));
					//GameInstance->SendTankHit(ProjectileDamage*DamageRatio);
				}
			}
		}


	}

	//UPrimitiveComponent* Primitive = InsideActor->GetRootPrimitiveComponent();
	//Primitive->IsSimulatingPhysics();
	//if(Primitive->IsSimulatingPhysics())
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, InsideActor->GetName());
	//InsideActor->GetRootPrimitiveComponent()->AddRadialImpulse(Origin, RadialRadius, ImpulseStrength, Falloff, false);
}



void AHomingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		
		
	}

	FireImpulse();

	

	AMultiGameMode* const GameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//��Ƽ����
	if (GameMode) {
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());

		GameInstance->SendExplosion();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), true)->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));


	Destroy();
}

void AHomingProjectile::SetHomingTarget(AActor* HomingTarget)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "SetHomingTarget Call!!");
	if (HomingTarget) {
		ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		ALOTMultiPlayer* MultiParent = Cast<ALOTMultiPlayer>(ParentTank);
		MultiParent->SetbIsShoot(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, HomingTarget->GetName() +"SetHomingTarget Success!!");
		
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "SetHomingTarget Fail!!");
}