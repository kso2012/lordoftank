// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Pawn/LOTPlayer.h"
#include "Weapon/ArmorPiercingProjectile.h"





AArmorPiercingProjectile::AArmorPiercingProjectile()
{

	CollisionComp->OnComponentHit.AddDynamic(this, &AArmorPiercingProjectile::OnHit);		

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/LOTAssets/TankAssets/Meshes/ArmorPiercingAmmo.ArmorPiercingAmmo'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);

	ProjectileDamage = 20.f;

}


void AArmorPiercingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->IsA(ALOTPlayer::StaticClass())) {
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Damage Player!");
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			Cast<ALOTPlayer>(OtherActor)->ApplyDamage(ProjectileDamage);
		}
		Destroy();
	}
}