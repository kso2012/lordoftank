// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Projectile.h"
#include "EmpProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API AEmpProjectile : public AProjectile
{
	GENERATED_BODY()
	
	void BeginPlay();

	void AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel);

	UFUNCTION(BlueprintCallable, Category = "Physics|Components|RadialForce")
		virtual void AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType);

	void UpdateCollisionObjectQueryParams();

	void FireImpulse();

	float RadialRadius;

	float ImpulseStrength;

protected:

	FCollisionObjectQueryParams CollisionObjectQueryParams;

	UPROPERTY(EditAnywhere, Category = RadialForceComponent)
		TArray<TEnumAsByte<enum EObjectTypeQuery> > ObjectTypesToAffect;




public:
	AEmpProjectile();



	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


	
	
};
