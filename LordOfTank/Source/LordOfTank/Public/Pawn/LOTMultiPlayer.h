// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WheeledVehicle.h"
#include "LOTMultiPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LORDOFTANK_API ALOTMultiPlayer : public AWheeledVehicle
{
	GENERATED_BODY()
		//��ũ��ü �޽�
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* TurretMesh;

	//��ũ���� �޽�
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* BarrelMesh;

	//�ѱ���ġ
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;


	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	//�̵���� ī�޶�
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* MoveModeCamera;

	//��ݸ�� ī�޶�
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FireModeCamera;



public:

	ALOTMultiPlayer();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Val);

	void MoveRight(float Val);

	//void OnResetVR();

	//////void SetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent);
	////UFUNCTION(BlueprintCallable, Category = "CustomFunc")
	////	void ToCallSetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent);


	//UFUNCTION()
	//	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	////UFUNCTION(BlueprintCallable, Category = "Animation")
	////		UWheeledVehicleMovementComponent* 


	//��ݹ�ư ������ ��
	//void FireStart();
	////��ݹ�ư ������ ��
	//void FireEnd();
	////��������� visible����
	//void FireMode();
	////�κ��丮����
	//void SetDefaultInvetory();
	////��ũ���� �������� ����.
	//UFUNCTION()
	//	void ApplyDamage(float damage);

private:


	////���� źȯ�� ����Ű�� ������,TSubclassOf�� ����ϴ� ������ <Ÿ��>�� ����ϴ� Ŭ������ �Ҵ��ϰ� �ϱ� ����.
	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
	//	TSubclassOf<class AProjectile> CurrentProjectile;
	//TArray<UParticleSystemComponent*> BeamArray;

	////���� ����ִ� �迭
	////źȯ�� ���� �κ��丮
	//UPROPERTY(EditDefaultsOnly, Category = Inventory)
	//	TArray<TSubclassOf<class AProjectile>> ProjectileInventory;
	////��ݸ���� �� Barrel�� Gun �޽� ��ȯ.
	void ChangeFiremodeBody();

	//FVector GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time);

	//void DrawTrajectory();

	//void ClearBeam();

	//void RaisePower();

	////��ư Ŭ�� �� ��������
	//void ChangeCamera(bool bIsFireMode);

	////���� �׸��� �Լ�
	//void DrawBeam(FVector StartLocation, FVector EndLocation);

	//�����̻��Ͽ��� �Ѱ��� ����Ÿ��
	class AActor* HomingTarget;


	//��ݸ���ΰ�?
	bool bIsFireMode;
	//�ִ�ü��
	float MaxHealth;
	//�ִ�ǵ差
	float MaxShield;
	//����ü��
	float CurrentHealth;
	//����AP
	float AP;

	float RaisingRate;

	float MinShootingPower;

	float MaxShootingPower;

	//�߻� �Ŀ�
	float CurShootingPower;

	//�߻����ΰ�?
	bool bIsShoot;

	bool bIsPushFire;



	
};
