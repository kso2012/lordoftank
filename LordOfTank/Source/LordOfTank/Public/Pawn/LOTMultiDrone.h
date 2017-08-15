// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "LOTMultiDrone.generated.h"

UCLASS()
class LORDOFTANK_API ALOTMultiDrone : public APawn
{
	GENERATED_BODY()
		//UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		//class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* BeamCollisionComp;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh1;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh2;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh3;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh4;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh5;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh6;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh7;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh8;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh9;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh10;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh11;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh12;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh13;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh14;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh15;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh16;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh17;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh18;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh19;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh20;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh21;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh22;
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BabylonMesh23;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BeamMesh;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm2;

	//�̵���� ī�޶�
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	//Ž����� ī�޶�
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* DetectCamera;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* CrossHair;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* CockPit;


protected:

	virtual void BeginPlay() override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;




private:


	UPROPERTY(Category = Plane, EditAnywhere)
		float Acceleration;


	UPROPERTY(Category = Plane, EditAnywhere)
		float TurnSpeed;


	UPROPERTY(Category = Pitch, EditAnywhere)
		float MaxSpeed;


	UPROPERTY(Category = Yaw, EditAnywhere)
		float MinSpeed;

	float DecreaseAccel;

	UParticleSystemComponent* Beam;

	UParticleSystemComponent* EmpEffect;

	UAudioComponent* EmpSound;


	float CurrentForwardSpeed;


	float CurrentUpwardSpeed;


	float CurrentYawSpeed;


	float CurrentPitchSpeed;


	float CurrentRollSpeed;

	uint32 bHasInputUpward : 1;

	uint32 bHasInputForward : 1;

	uint32 bIsDetectMode : 1;

	uint32 bIsPullMode : 1;

	float MoveAP;

//	class AActor* HomingTarget;

	//������ ���� �ִϸ��̼� ����.
	void SetAnim();
	//Ÿ���ý� �������� �׷��ִ� �Լ�.
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	void ClearBeam();

	void ChangePawn();

public:


	ALOTMultiDrone();
	//���Ϸ� ������ �� �Է�ó��
	void MoveUpwardInput(float Val);

	//�յڷ� ������ �� �Է�ó��
	void MoveForwardInput(float val);

	//�¿�� ȸ���� �� �Է�ó��
	void MoveRightInput(float Val);

	virtual void Tick(float DeltaTime) override;
	//Ÿ�ټ���
	void SetTarget();

	void PullActor(float time);

	void RightClickPress();

	void RightClickRelease();

	void SetBeamVisible(bool bBeam);

	//FORCEINLINE class AActor* GetHomingTarget() const { return HomingTarget; }

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void PossessCall();

	void DetectMode();

	void SetUI(bool bIsPlayer);

	void SetEmp(bool EmpState);

	float FloatingAnim;

};
