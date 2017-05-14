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
	AmmoMesh->SetRelativeLocation(FVector(-600.0f, 0.0f, 0.0f));
	AmmoMesh->SetRelativeScale3D(FVector(0.5f,0.5f,0.5));

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'"));
	//TrailParticle->SetTemplate(TrailParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'"));
	FlareParticle->SetTemplate(FlareParticleAsset.Object);
	FlareParticle->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_ExplosionWithShrapnel.P_ExplosionWithShrapnel'"));
	ExplosionParticle = ExplosionParticleAsset.Object;
	
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/LOTAssets/TankAssets/Audio/Flyby_Cue.Flyby_Cue"));
	FlySoundComponent->SetSound(SoundCue.Object);

	AddCollisionChannelToAffect(ECC_MAX);
	//AddCollisionChannelToAffect(ECC_Pawn);
	//AddCollisionChannelToAffect(ECC_PhysicsBody);
	//AddCollisionChannelToAffect(ECC_Vehicle);
	//AddCollisionChannelToAffect(ECC_Destructible);
	UpdateCollisionObjectQueryParams();

	RadialRadius = 1000.f; //폭발 반경
	ImpulseStrength = 1000000.f;
	ProjectileDamage = 50.f;
	//ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 100.f;

}

void AHomingProjectile::BeginPlay()
{
	Super::BeginPlay();

	FlySoundComponent->Play();


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
		ATrainingMode* const TrainingGameMode = Cast<ATrainingMode>(UGameplayStatics::GetGameMode(GetWorld()));
		//멀티게임
		if (GameMode)
		{

			if (ALOTMultiPlayer* const Test = Cast<ALOTMultiPlayer>(InsideActor)) {
				//쏜 자신에게 맞았을 경우 
				if ((bIsFireEnemy == false && Test == ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;

					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("내가쏜거 내가맞음 %f!!"), ProjectileDamage*DamageRatio));
					GameInstance->SendTankHit(ProjectileDamage*DamageRatio, PROJECTILE_HOMING);
				}
				//적에게 맞았을 경우
				else if ((bIsFireEnemy == true && Test != ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;
					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이쏜거 내가맞음 %f!!"), ProjectileDamage*DamageRatio));
					GameInstance->SendTankHit(ProjectileDamage*DamageRatio, PROJECTILE_HOMING);
				}
			}
		}
		//싱글게임
		else if (SingleMode == UGameplayStatics::GetGameMode(GetWorld()) || TrainingGameMode) {

			if (ALOTPlayer* const Test = Cast<ALOTPlayer>(InsideActor)) {
				//쏜 자신에게 맞았을 경우 
				if ((bIsFireEnemy == false && Test == ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;

					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("내가쏜거 내가맞음 %f!!"), ProjectileDamage*DamageRatio));
					//GameInstance->SendTankHit(ProjectileDamage*DamageRatio);
					Test->ApplyDamage(ProjectileDamage*DamageRatio, PROJECTILE_HOMING);
				}
				//적에게 맞았을 경우
				else if ((Test != ParentTank))
				{
					FVector ActorLocation = InsideActor->GetActorLocation();
					float CenterToLength = UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X)
						+ UKismetMathLibrary::Square(Origin.Y - ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
					if (CenterToLength > RadialRadius)
						CenterToLength = RadialRadius;
					float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("적이쏜거 내가맞음 %f!!"), ProjectileDamage*DamageRatio));
					//GameInstance->SendTankHit(ProjectileDamage*DamageRatio);
					Test->ApplyDamage(ProjectileDamage*DamageRatio, PROJECTILE_HOMING);
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
	//멀티게임
	if (GameMode) {
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
		ALOTMultiPlayer* MultiParent = Cast<ALOTMultiPlayer>(ParentTank);
		MultiParent->SetbIsShoot(false);
		GameInstance->SendExplosion();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), true)->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LoadObject<USoundCue>(nullptr, TEXT("/Game/LOTAssets/TankAssets/Audio/Explosion_Cue.Explosion_Cue")),
		GetActorLocation(), GetActorRotation());

	Destroy();
}

void AHomingProjectile::SetHomingTarget(AActor* HomingTarget,float Magnitude)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "SetHomingTarget Call!!");
	if (HomingTarget) {
		ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		ProjectileMovement->HomingAccelerationMagnitude = Magnitude;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, HomingTarget->GetName() +"SetHomingTarget Success!!");

	}
	else
		;//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "SetHomingTarget Fail!!");
}

void AHomingProjectile::ActivateHoming()
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->bIsHomingProjectile = true;
}