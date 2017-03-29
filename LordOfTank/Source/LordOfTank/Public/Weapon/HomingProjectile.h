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

	/** Add a collision channel for this radial force to affect */
	void AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel);

	/** Add an object type for this radial force to affect */
	UFUNCTION(BlueprintCallable, Category = "Physics|Components|RadialForce")
		virtual void AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType);


	/** Update CollisionObjectQueryParams from ObjectTypesToAffect */
	void UpdateCollisionObjectQueryParams();

	void FireImpulse();

	float RadialRadius;

	float ImpulseStrength;

protected:
	/** Cached object query params derived from ObjectTypesToAffect */
	FCollisionObjectQueryParams CollisionObjectQueryParams;

	/** The object types that are affected by this radial force */
	UPROPERTY(EditAnywhere, Category = RadialForceComponent)
		TArray<TEnumAsByte<enum EObjectTypeQuery> > ObjectTypesToAffect;


public:
	AHomingProjectile();

	/** called when projectile hits something */
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void SetHomingTarget(AActor* HomingTarget) override;
	
	
};
