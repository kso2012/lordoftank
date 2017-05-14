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
		class USpringArmComponent* SpringArm2;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

		//이동모드 카메라
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
		//탐지모드 카메라
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* DetectCamera;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* CrossHair;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UChildActorComponent* UI;


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

	//날개와 몸통 애니메이션 지정.
	void SetAnim();
	//타겟팅시 레이져를 그려주는 함수.
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	void ClearBeam();

	void ChangePawn();

	bool timercounteron;

	float timercounter;


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

	bool PossessDrone;

	bool bDetectMode;

	
	/////////////////////////////
	// AI관련 

private:

	// 플레이어인지 AI인지 판단하는 bool, true면 플레이어
	bool isNotAI;

	// 드론이 바라보는 인지 범위로 사용할 collision box
	UBoxComponent* ViewBox;

	int PawnNum;

	// collision box 초기화 함수
	void SetViewBoxLocation();

	float AP;

	float MoveAP; 
	
	void TankPossess();


public:

	void SetSingleUI(bool);
	// 게임모드에서 이 탱크가 AI인지 아닌지를 설정해주는 함수
	void SetisNotAI(bool isntAI) { isNotAI = isntAI; }

	// !IsNotAI를 반환, true이면 AI
	bool GetisAI() { return !isNotAI; }

	int GetPawnNum() { return PawnNum; }

	// Drone의 인지 범위 내에 탱크가 들어왔는지 판단하는 변수
	int DecideCollisionState;

	// ViewBox에 충돌된 Actor를 저장하는 변수
	AActor * CollisionActor;
	
	// ViewBox에 충돌되었을 때 호출되는 함수
	UFUNCTION()
		void DroneFindEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ViewBox에서 벗어났을 때 호출되는 함수
	UFUNCTION()
		void DroneLostEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	// ViewBox를 Activate
	void OnViewBox() {
		ViewBox->Activate();
		ViewBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ViewBox->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
		
	}

	// ViewBox를 Deactivate
	void OffViewBox() {
		ViewBox->Deactivate();
		ViewBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ViewBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// 입력된 값을 이용해 드론을 회전
	void RotateDrone(FRotator);

	void CommandMoveForward(float val) { MoveForwardInput(val); }

	float ReturnDroneSpeed() { return CurrentForwardSpeed; }

	void SetDroneSpeed() { CurrentForwardSpeed = 0.f; }


	FVector ReturnMeshLocation() { return Root->GetComponentLocation(); }
	FVector ReturnForwardVector() { return Root->GetForwardVector(); }


	void SetAP(float ap) { AP = ap; }
	float ReturnAP() { return AP; }
};
