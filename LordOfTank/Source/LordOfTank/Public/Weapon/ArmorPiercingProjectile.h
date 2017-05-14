// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Projectile.h"
#include "ArmorPiercingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API AArmorPiercingProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AArmorPiercingProjectile();

	void BeginPlay();


	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	
};
