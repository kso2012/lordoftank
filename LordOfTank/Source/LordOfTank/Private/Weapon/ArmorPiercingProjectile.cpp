// Fill out your copyright notice in the Description page of Project Settings.

#include "LordOfTank.h"
#include "Pawn/LOTPlayer.h"
#include "Weapon/ArmorPiercingProjectile.h"
#include "Weapon/Projectile.h"





AArmorPiercingProjectile::AArmorPiercingProjectile()
{

	CollisionComp->OnComponentHit.AddDynamic(this, &AArmorPiercingProjectile::OnHit);		
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoStaticMesh(TEXT("StaticMesh'/Game/ProjectilesPack/Meshes/Projectiles/Rockets/SM_Rocket_01.SM_Rocket_01'"));
	AmmoMesh->SetStaticMesh(AmmoStaticMesh.Object);
	AmmoMesh->SetRelativeLocation(FVector(-300.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'"));
	TrailParticle->SetTemplate(TrailParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'"));
	FlareParticle->SetTemplate(FlareParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleAsset(TEXT("ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke.P_Smoke'"));
	ExplosionParticle = ExplosionParticleAsset.Object;

	ProjectileDamage = 40.f;

}


void AArmorPiercingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{

		AMultiGameMode* const MultiGameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		AMultiGameMode* const SingleGameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MultiGameMode)
		{
			ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
			if (ALOTMultiPlayer* const Test = Cast<ALOTMultiPlayer>(OtherActor)) {
				//쏜 자신에게 맞았을 경우 
				if ((bIsFireEnemy == false && Test == ParentTank))
				{
					OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
					GameInstance->SendTankHit(ProjectileDamage, PROJECTILE_ARMORPIERCING);
				}
				//적에게 맞았을 경우
				else if ((bIsFireEnemy == true && Test != ParentTank))
				{
					OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
					GameInstance->SendTankHit(ProjectileDamage, PROJECTILE_ARMORPIERCING);
				}
			}


		}

		else if (SingleGameMode) {
			if (OtherActor->IsA(ALOTPlayer::StaticClass())) {
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Damage Player!");
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
				Cast<ALOTPlayer>(OtherActor)->ApplyDamage(ProjectileDamage);
			}
		}
		
		if (UDestructibleComponent* DestructibleComponent = Cast<UDestructibleComponent>(OtherComp))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, " Destructorble!");
			DestructibleComponent->ApplyRadiusDamage(10000000.f, OtherComp->GetComponentLocation(),900.f, 10000000.f, RIF_Constant);
			//DestructibleComponent->ApplyDamage(10000000.f, OtherComp->GetComponentLocation(), GetActorForwardVector(), 10000000.f);
		}
	}

	AMultiGameMode* const GameMode = Cast<AMultiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//멀티게임
	if (GameMode) {
		ULOTGameInstance* const GameInstance = Cast<ULOTGameInstance>(GetGameInstance());
		ALOTMultiPlayer* MultiParent = Cast<ALOTMultiPlayer>(ParentTank);
		MultiParent->SetbIsShoot(false);
		GameInstance->SendExplosion();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), true)->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));





	Destroy();
}