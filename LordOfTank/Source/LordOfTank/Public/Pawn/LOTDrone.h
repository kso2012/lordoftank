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

		//비행기 몸통 메쉬
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PlaneMesh;


	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

		//이동모드 카메라
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
		//탐지모드 카메라
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* DetectCamera;


protected:

	virtual void BeginPlay() override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; 


	void ThrustInput(float Val);

	//상하로 움직일 때 입력처리
	void MoveUpwardInput(float Val);

	//앞뒤로 움직일 때 입력처리
	void MoveForwardInput(float val);

	//좌우로 회전할 때 입력처리
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
	//타겟설정
	void SetTarget();

	FORCEINLINE class AActor* GetHomingTarget() const { return HomingTarget; }
	
	//FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }

	//FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }

	//FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	void DetectMode();

	
	
	
};
