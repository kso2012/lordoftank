// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Pawn/LOTPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/CommonProjectile.h"




ACommonProjectile::ACommonProjectile()
{
	
	CollisionComp->OnComponentHit.AddDynamic(this, &ACommonProjectile::OnHit);		// set up a notification for when this component hits something blocking


	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/LOTAssets/TankAssets/Meshes/CommonAmmo.CommonAmmo'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);
	
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(TEXT("ParticleSystem'/Game/LOTAssets/TankAssets/Particles/PT_ProjectileTrail.PT_ProjectileTrail'"));
	TrailParticle->SetTemplate(TrailParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleAsset(TEXT("ParticleSystem'/Game/LOTAssets/TankAssets/Particles/PT_LargeExplosion.PT_LargeExplosion'"));
	ExplosionParticle = ExplosionParticleAsset.Object;

	AddCollisionChannelToAffect(ECC_MAX);
	//AddCollisionChannelToAffect(ECC_Pawn);
	//AddCollisionChannelToAffect(ECC_PhysicsBody);
	//AddCollisionChannelToAffect(ECC_Vehicle);
	//AddCollisionChannelToAffect(ECC_Destructible);
	UpdateCollisionObjectQueryParams();

	RadialRadius = 1000.f; //Æø¹ß ¹Ý°æ
	ImpulseStrength = 1000000.f;
	ProjectileDamage = 10.f;


}

void ACommonProjectile::AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType)
{
	ObjectTypesToAffect.AddUnique(ObjectType);
	UpdateCollisionObjectQueryParams();
}

void ACommonProjectile::AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel)
{
	EObjectTypeQuery ObjectType = UEngineTypes::ConvertToObjectType(CollisionChannel);
	if (ObjectType != ObjectTypeQuery_MAX)
	{
		AddObjectTypeToAffect(ObjectType);
	}
}

void ACommonProjectile::UpdateCollisionObjectQueryParams()
{
	CollisionObjectQueryParams = FCollisionObjectQueryParams(ObjectTypesToAffect);
}


void ACommonProjectile::FireImpulse()
{
	ERadialImpulseFalloff Falloff;
	Falloff = RIF_Linear;
	const FVector Origin = GetActorLocation();

	// Find objects within the sphere
	static FName FireImpulseOverlapName = FName(TEXT("FireImpulseOverlap"));
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(FireImpulseOverlapName, false);
	Params.bTraceAsyncScene = true; // want to hurt stuff in async scene

									// Ignore owner actor if desired

	Params.AddIgnoredActor(GetOwner());
	int count = 0;

	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(RadialRadius), Params);
	// A component can have multiple physics presences (e.g. destructible mesh components).
	// The component should handle the radial force for all of the physics objects it contains
	// so here we grab all of the unique components to avoid applying impulses more than once.
	TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
	TArray<AActor*, TInlineAllocator<1>> AffectedActors;
	AffectedComponents.Reserve(Overlaps.Num());

	for (FOverlapResult& OverlapResult : Overlaps)
	{
		if (UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
		{
			
			AffectedComponents.AddUnique(PrimitiveComponent);
		
		}

		if (AActor* InsideActor= OverlapResult.Actor.Get())
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
				DestructibleComponent->ApplyRadiusDamage(ImpulseStrength, Origin, RadialRadius, ImpulseStrength, Falloff == RIF_Constant );
			}
		}


		PrimitiveComponent->AddRadialImpulse(Origin, RadialRadius, ImpulseStrength, Falloff, false);
	}

	for (AActor* InsideActor : AffectedActors)
	{
		if (ALOTPlayer* const Test = Cast<ALOTPlayer>(InsideActor)) {
			FVector ActorLocation = InsideActor->GetActorLocation();
			float CenterToLength=UKismetMathLibrary::Sqrt(UKismetMathLibrary::Square(Origin.X - ActorLocation.X) 
				+ UKismetMathLibrary::Square(Origin.Y-ActorLocation.Y) + UKismetMathLibrary::Square(Origin.Z - ActorLocation.Z));
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%f"), CenterToLength));
			if (CenterToLength > RadialRadius)
				CenterToLength = RadialRadius;

			float DamageRatio = (1.0f - (CenterToLength / RadialRadius));
			
			Test->TakeDamage(ProjectileDamage*DamageRatio);
		}
	}

		//UPrimitiveComponent* Primitive = InsideActor->GetRootPrimitiveComponent();
		//Primitive->IsSimulatingPhysics();
		//if(Primitive->IsSimulatingPhysics())
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, InsideActor->GetName());
		//InsideActor->GetRootPrimitiveComponent()->AddRadialImpulse(Origin, RadialRadius, ImpulseStrength, Falloff, false);
}


void ACommonProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) )
	{
		FireImpulse();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), true);


		Destroy();
	}
}