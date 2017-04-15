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

	ALOTPlayer();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Val);

	void MoveRight(float Val);

	void OnResetVR();

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
	//�����
	void Fire();
	//��������� visible����
	void FireMode();
	//�κ��丮����
	void SetDefaultInvetory();
	//��ũ���� �������� ����.
	UFUNCTION()
	void ApplyDamage(float damage);

private:
	//���� źȯ�� ����Ű�� ������,TSubclassOf�� ����ϴ� ������ <Ÿ��>�� ����ϴ� Ŭ������ �Ҵ��ϰ� �ϱ� ����.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> CurrentProjectile;

	//źȯ�� ���� �κ��丮
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AProjectile>> ProjectileInventory;
	//��ݸ���� �� Barrel�� Gun �޽� ��ȯ.
	void ChangeFiremodeBody();

	FVector GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time);

	void DrawTrajectory();

	

	//��ư Ŭ�� �� ��������
	void ChangeCamera(bool bIsFireMode);

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





};
