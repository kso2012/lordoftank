// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Weapon/HomingProjectile.h"




AHomingProjectile::AHomingProjectile()
{
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AHomingProjectile::OnHit);		


	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/LOTAssets/TankAssets/Meshes/ArmorPiercingAmmo.ArmorPiercingAmmo'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);
	


	
	

}


void AHomingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Damage Player!");
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		Destroy();

	}
}

void AHomingProjectile::SetHomingTarget(AActor* HomingTarget)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "SetHomingTarget Call!!");
	if (HomingTarget) {
		ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, HomingTarget->GetName() +"SetHomingTarget Success!!");
		
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "SetHomingTarget Fail!!");
}