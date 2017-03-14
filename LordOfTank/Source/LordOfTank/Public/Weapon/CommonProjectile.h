// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Projectile.h"
#include "CommonProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API ACommonProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	ACommonProjectile();

	/** called when projectile hits something */
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	
};
