// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Weapon/CommonProjectile.h"




ACommonProjectile::ACommonProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");				//충돌체의 프로필 이름을 설정한다.
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->OnComponentHit.AddDynamic(this, &ACommonProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Set as root component
	RootComponent = CollisionComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/LOTAssets/TankAssets/Meshes/CommonAmmo.CommonAmmo'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);
	AmmoMesh->SetupAttachment(RootComponent);
	AmmoMesh->SetRelativeRotation(FRotator(90.f, 0.0f, 0.0f));







	//프로젝트타일무브먼트컴포넌트는 물리적인 오브젝트 위치를 나타내는 씬 컴포넌트가 아니기 때문에 붙이거나 하지 않는다.
	// Use a ProjectileMovementComponent to govern this projectile's movement
	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 8000.f;
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;	//속도에 따라서 탄환을 회전시키고 싶을 때 사용한다.
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 1.f;

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;

}


void ACommonProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) )
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Damage Player!");
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		//OtherActor->Destroy();
		Destroy();
	}
}