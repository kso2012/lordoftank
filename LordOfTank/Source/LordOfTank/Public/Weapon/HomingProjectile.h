// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Projectile.h"
#include "HomingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API AHomingProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AHomingProjectile();

	/** called when projectile hits something */
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void SetHomingTarget(AActor* HomingTarget) override;
	
	
};
