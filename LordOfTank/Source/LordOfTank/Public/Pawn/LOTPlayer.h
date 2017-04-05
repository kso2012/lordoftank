// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WheeledVehicle.h"
#include "LOTPlayer.generated.h"

/**
 * 
 */

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;

//UENUM(BlueprintType)
//enum class EPlayerState
//{
//	EMain,
//	EPlaying,
//	EWon,
//	EUnknown
//};

UCLASS()
class LORDOFTANK_API ALOTPlayer : public AWheeledVehicle
{
	GENERATED_BODY()


	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* TurretMesh;


	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* BarrelMesh;


	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;


	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;


	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* MoveModeCamera;


	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FireModeCamera;



public:

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	
	ALOTPlayer();

	virtual void BeginPlay();


	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	
	void MoveForward(float Val);

	
	void MoveRight(float Val);


	void OnResetVR();


	

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Turn(float Val);

	void Fire();

	void FireMode();

	void SetDefaultInvetory();

	void One();

	void Two();
	UFUNCTION()
	void ApplyDamage(float damage);

	

private:

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActor> CurrentProjectile;


	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AActor>> ProjectileInventory;

	class AActor* HomingTarget;

	uint32 bIsFireMode : 1;

	float MaxHealth;

	float MaxShield;


	float CurrentHealth;





};
