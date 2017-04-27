// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LOTPlayer.h"
#include "Projectile.generated.h"

UCLASS()
class LORDOFTANK_API AProjectile : public AActor
{
	GENERATED_BODY()
protected:


	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollisionComp;

	//�߻�ü �̵��� ���ؼ��� ������ƮŸ�Ϲ����Ʈ������Ʈ�� ����ؾ��Ѵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;


	/*�߻�ü �޽�*/
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
	
public:	

	AProjectile();


	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
		
	virtual void SetHomingTarget(AActor* HomingTarget);

protected:

	virtual void BeginPlay() override;
	//ź�� ��
	float AmmoCount;
	//ź�� ������
	float ProjectileDamage;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetInitialVelocity(FVector velocity);

	ALOTPlayer *ParentTank;

	void GetTank(ALOTPlayer * tank) { ParentTank = tank; }

	void SendMessage() { ParentTank->ChangeTurn(); }
};
