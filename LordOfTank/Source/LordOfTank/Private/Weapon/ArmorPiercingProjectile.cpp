// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Pawn/LOTPlayer.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/Projectile.h"





AArmorPiercingProjectile::AArmorPiercingProjectile()
{

	CollisionComp->OnComponentHit.AddDynamic(this, &AArmorPiercingProjectile::OnHit);		

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/LOTAssets/TankAssets/Meshes/ArmorPiercingAmmo.ArmorPiercingAmmo'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'"));
	TrailParticle->SetTemplate(TrailParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'"));
	FlareParticle->SetTemplate(FlareParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke.P_Smoke'"));
	ExplosionParticle = ExplosionParticleAsset.Object;

	ProjectileDamage = 20.f;

}


void AArmorPiercingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->IsA(ALOTPlayer::StaticClass())) {
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Damage Player!");
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			Cast<ALOTPlayer>(OtherActor)->ApplyDamage(ProjectileDamage);
		}
		
		if (UDestructibleComponent* DestructibleComponent = Cast<UDestructibleComponent>(OtherComp))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, " Destructorble!");
			DestructibleComponent->ApplyRadiusDamage(10000000.f, OtherComp->GetComponentLocation(),900.f, 10000000.f, RIF_Constant);
			//DestructibleComponent->ApplyDamage(10000000.f, OtherComp->GetComponentLocation(), GetActorForwardVector(), 10000000.f);
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), true)->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));


	AProjectile::SendMessage();


	Destroy();
}