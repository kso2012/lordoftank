// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/Projectile.h"
#include "LordOfTankGameModeBase.h"
#include "CommonProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API ACommonProjectile : public AProjectile
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

	ALordOfTankGameModeBase* Single ;
	
	void MakeInvisability();

protected:

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	
	UPROPERTY(EditAnywhere, Category=RadialForceComponent)
		TArray<TEnumAsByte<enum EObjectTypeQuery> > ObjectTypesToAffect;


	

public:
	ACommonProjectile();

	

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	

	
};
