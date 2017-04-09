// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
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

		//����� ���� �޽�
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PlaneMesh;


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

	
	
	
};
