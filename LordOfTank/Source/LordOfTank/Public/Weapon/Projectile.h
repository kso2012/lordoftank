// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LOTPlayer.h"
#include "GameMode/MultiGameMode.h"
#include "GameMode/TrainingMode.h"
#include "LordOfTankGameModeBase.h"
#include "LOTGameInstance.h"
#include "Projectile.generated.h"

UCLASS()
class LORDOFTANK_API AProjectile : public AActor
{
	GENERATED_BODY()
protected:


	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollisionComp;

	//발사체 이동에 관해서는 프로젝트타일무브먼트컴포넌트를 사용해야한다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;


	/*발사체 메쉬*/
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* AmmoMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UParticleSystemComponent* TrailParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UParticleSystemComponent* FlareParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UParticleSystem* ExplosionParticle;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;


	//탱크 엔진 사운드
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* FlySoundComponent;
	
public:	

	AProjectile();


	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

	virtual void SetHomingTarget(AActor* HomingTarget, float Magnitude);

protected:

	virtual void BeginPlay() override;
	//탄알 수
	float AmmoCount;
	//탄알 데미지
	float ProjectileDamage;

	bool bIsFireEnemy;





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetInitialVelocity(FVector velocity);

	AActor* ParentTank;
	AActor* ParentDrone;

	void GetTank(ALOTPlayer * tank) { ParentTank = tank; }

	//void SendMessage() { ParentTank->ChangeTurn(); }

	void SetEnemyFire(bool bEnemy);
};
