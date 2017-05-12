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


	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineSoundComponent;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* CrossHair;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* UI;

public:

	ALOTPlayer();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Val);

	void MoveRight(float Val);

	void OnResetVR();

	////void SetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent);
	//UFUNCTION(BlueprintCallable, Category = "CustomFunc")
	//	void ToCallSetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent);


	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//UFUNCTION(BlueprintCallable, Category = "Animation")
//		UWheeledVehicleMovementComponent* 

	
	//��ݹ�ư ������ ��
	void FireStart();
	//��ݹ�ư ������ ��
	void FireEnd();
	//��������� visible����
	void FireMode();
	//�κ��丮����
	void SetDefaultInvetory();
	//��ũ���� �������� ����.
	UFUNCTION()
	void ApplyDamage(float damage);
	
	//AP�� ��ȯ�ϴ� �Լ�
	float getAP() {
		return AP;
	}

	

	float getPower() { return CurShootingPower; }

	void SetTurn(bool b) { myTurn = b; }

	bool GetTurn() { return myTurn; }

	void ChangeTurn();

	bool PossessTank;


	

private:


	//���� źȯ�� ����Ű�� ������,TSubclassOf�� ����ϴ� ������ <Ÿ��>�� ����ϴ� Ŭ������ �Ҵ��ϰ� �ϱ� ����.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> CurrentProjectile;
	TArray<UParticleSystemComponent*> BeamArray;

	//���� ����ִ� �迭
	//źȯ�� ���� �κ��丮
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AProjectile>> ProjectileInventory;
	//��ݸ���� �� Barrel�� Gun �޽� ��ȯ.
	void ChangeFiremodeBody();

	FVector GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time);

	void DrawTrajectory();

	void ClearBeam();

	void RaisePower();

	//��ư Ŭ�� �� ��������
	void ChangeCamera(bool bIsFireMode);

	//UParticleSystemComponent* TrajectoryBeam;
	
	//���� �׸��� �Լ�
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	void ChangePawn();

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

	float MoveAP;

	bool myTurn;

	float RaisingRate;

	float MinShootingPower;

	float MaxShootingPower;

	//�߻� �Ŀ�
	float CurShootingPower;

	//�߻����ΰ�?
	bool bIsShoot;

	bool bIsPushFire;

	float timercounter;

	bool timercounteron;


	/////////////////////////////
	// AI���� 
private:

	// �÷��̾����� AI���� �Ǵ��ϴ� bool, true�� �÷��̾�
	bool isNotAI;

	// AI�� ���� ��� ���� �Լ�, �Ŀ����� �޾Ƽ� FireEnd() �Լ��� ����
	void ShootAI(float);


	void TurnAI();



	int PawnNum;

	// AI�� �÷��̾��� ��ġ�� �����ϱ� ���� ����
	FVector EnemyLocation;


	void DronePossess();

public:

	void SetSingleUI(bool);
	// AI�� ������ ȸ����ų �� ViewBox�� �浹�� �Ǿ����� Ȯ���ϴ� ����
	int TurretAim;

	// ViewBox�� �浹�� Actor�� �����ϴ� ����
	AActor * CollisionActor;

	// ���Ӹ�忡�� ShootAI�� ȣ���ϱ� ���� �Լ�, ���Ӹ�忡�� ��ũ���� �Ŀ����� ������
	void CommandShoot(float power) { ShootAI(power); }
	void CommandTurn() { TurnAI(); }
	
	// ���Ӹ�忡�� �� ��ũ�� AI���� �ƴ����� �������ִ� �Լ�
	void SetisNotAI(bool isntAI) { isNotAI = isntAI; }

	// !IsNotAI�� ��ȯ, true�̸� AI
	bool GetisAI() { return !isNotAI; }

	
	int GetPawnNum() { return PawnNum; }

	// Camera�� �ܺο��� ����ϱ� ���� ��ȯ
	UCameraComponent* ReturnCamera() { return MoveModeCamera; }


	// �÷��̾��� ��ġ�� �� AI�� �����ϱ� ���� ��ġ�� �޾� �����Ű�� �Լ�
	void SetEnemyLocation(FVector location) { EnemyLocation = location; }

	// �Էµ� ���� �̿��� �ͷ��� ȸ��
	void RotateTurret(FRotator);



	// ���� �߻������� �ƴ����� ���Ӹ�忡�� Ȯ���ϱ� ���� ���� ����
	bool GetbIsShoot() { return bIsShoot; }



	// AI�� ��� �������� ȸ���ؾ����� ����ϱ� ���� �޽��� ��ġ�� ������ forwardVector�� ����
	FVector ReturnMeshLocation() { return GetMesh()->GetComponentLocation(); }
	FVector ReturnTurretForwardVector() { return TurretMesh->GetForwardVector(); }

	void SetAim(float distance);

	bool RightShot;

	ALOTPlayer* Player;

	bool bIsWaiting;

	bool bIsTestShot;

	int AimCount;

	void SetbIsShoot() { bIsShoot = false; }

	void SetAP(float ap) { AP = ap; }
	float ReturnAP() { return AP; }
	
};
