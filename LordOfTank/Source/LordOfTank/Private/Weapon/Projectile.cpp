// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Weapon/Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	// Use a sphere as a simple collision representation
	CollisionComp->InitSphereRadius(1.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");				
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	//CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//CollisionComp->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	RootComponent = CollisionComp;

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	AmmoMesh->SetupAttachment(RootComponent);
	AmmoMesh->SetRelativeRotation(FRotator(-90.f, 0.0f, 0.0f));
	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->AttachToComponent(AmmoMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("TrailLocation"));

	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionParticle"));
	

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 8000.f;
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 1.f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{

	Super::BeginPlay();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void AProjectile::SetHomingTarget(AActor* HomingTarget)
{
	;
}