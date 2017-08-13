// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WheeledVehicle.h"
#include "LOTMultiPlayer.generated.h"

#define P_COMMON		0
#define P_ARMORPIERCE	1
#define P_HOMING		2
#define P_EMP			3

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

	//��ũ ���� ����
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineSoundComponent;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* CrossHair;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* UI;



public:

	ALOTMultiPlayer();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Val);

	void MoveRight(float Val);

	void SetUI(bool bIsPlayer);

	void SetDead();

	void NextWeapon();

	void ExWeapon();

	//void OnResetVR();

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void PossessCall();
	
	
	void SetbIsShoot(bool IsShoot);

	//��ݹ�ư ������ ��
	void FireStart();
	////��ݹ�ư ������ ��
	void FireEnd();
	//��������� visible����
	void FireMode();

	void ChangeTurn() { bIsPushFire = false; bIsShoot = false; }
	////�κ��丮����
	void SetDefaultInvetory();

	void SetArmorPierceNum();

	void SetHomingNum();

	void SetEmpNum();

	void SetTurn(bool turn) { bIsMyTurn = turn; }

	UFUNCTION(BlueprintCallable, Category = "ProjectileNum")
		int ProjectileType() { return CurInventoryIndex; }

	UFUNCTION(BlueprintCallable, Category = "ProjectileNum")
		int ProjectileNum() { return HadProjectileNum[CurInventoryIndex]; }

	//������ �ִ� ź�� ������ ������ �迭
		int HadProjectileNum[4];

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileNum")
		int CurInventoryIndex;

private:


	////���� źȯ�� ����Ű�� ������,TSubclassOf�� ����ϴ� ������ <Ÿ��>�� ����ϴ� Ŭ������ �Ҵ��ϰ� �ϱ� ����.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> CurrentProjectile;
	TArray<UParticleSystemComponent*> BeamArray;

	////���� ����ִ� �迭
	////źȯ�� ���� �κ��丮
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AProjectile>> ProjectileInventory;

	
	////��ݸ���� �� Barrel�� Gun �޽� ��ȯ.
	void ChangeFiremodeBody();

	void ChangePawn();

	FVector GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time);

	void DrawTrajectory();

	void ClearBeam();

	void RaisePower();

	////��ư Ŭ�� �� ��������
	void ChangeCamera(bool bIsFireMode);

	////���� �׸��� �Լ�
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	
	class AActor* HomingTarget;


	//��ݸ���ΰ�?
	bool bIsFireMode;

	float RaisingRate;

	float MinShootingPower;

	float MaxShootingPower;

	//�߻� �Ŀ�
	float CurShootingPower;

	float CurForwardVal;

	float CurRightVal;

	float ExForwardVal;

	float ExRightVal;

	float MoveAP;

	bool bIsFireHoming;

	bool bIsSendForward;

	bool bIsSendRight;


	int Type;
	//�߻����ΰ�?
	bool bIsShoot;

	bool bIsPushFire;

	class AProjectile* TempActor;

	void TurnOver();

	float d_time;

	bool bIsMyTurn;



};
