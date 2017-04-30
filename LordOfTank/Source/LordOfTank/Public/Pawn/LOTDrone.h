// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "LOTDrone.generated.h"


enum class EDroneState
{
	EStop,
	EMoving
};


UCLASS()
class LORDOFTANK_API ALOTDrone : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;
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



	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

		//�̵���� ī�޶�
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
		//Ž����� ī�޶�
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* DetectCamera;


protected:

	virtual void BeginPlay() override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; 


	void ThrustInput(float Val);

	//���Ϸ� ������ �� �Է�ó��
	void MoveUpwardInput(float Val);

	//�յڷ� ������ �� �Է�ó��
	void MoveForwardInput(float val);

	//�¿�� ȸ���� �� �Է�ó��
	void MoveRightInput(float Val);

private:


	UPROPERTY(Category = Plane, EditAnywhere)
		float Acceleration;

	
	UPROPERTY(Category = Plane, EditAnywhere)
		float TurnSpeed;

	
	UPROPERTY(Category = Pitch, EditAnywhere)
		float MaxSpeed;


	UPROPERTY(Category = Yaw, EditAnywhere)
		float MinSpeed;

	UParticleSystemComponent* Beam;


	float CurrentForwardSpeed;

	
	float CurrentUpwardSpeed;


	float CurrentYawSpeed;


	float CurrentPitchSpeed;


	float CurrentRollSpeed;

	uint32 bHasInputUpward : 1;

	uint32 bHasInputForward : 1;
	
	uint32 bAcceleratedUpward : 1;

	uint32 bAcceleratedForward : 1;

	uint32 bIsDetectMode : 1;

	class AActor* HomingTarget;

	//������ ���� �ִϸ��̼� ����.
	void SetAnim();
	//Ÿ���ý� �������� �׷��ִ� �Լ�.
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	void ClearBeam();

	void ChangePawn();

public:


	ALOTDrone();

	virtual void Tick(float DeltaTime) override;
	//Ÿ�ټ���
	void SetTarget();

	FORCEINLINE class AActor* GetHomingTarget() const { return HomingTarget; }
	
	//FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }

	//FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }

	//FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	void DetectMode();

	bool PossessDrone;

	
	/////////////////////////////
	// AI���� 

private:

	bool isNotAI;

	UBoxComponent* ViewBox;

	void SetViewBoxLocation();

	int PawnNum;



public:

	void SetisNotAI(bool isntAI) { isNotAI = isntAI; }
	bool GetisAI() { return !isNotAI; }

	int GetPawnNum() { return PawnNum; }

	int DecideCollisionState;

	AActor * CollisionActor;
	
	
	UFUNCTION()
		void DroneFindEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DroneLostEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
