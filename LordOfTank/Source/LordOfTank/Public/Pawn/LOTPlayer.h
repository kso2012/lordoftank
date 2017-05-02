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

		//탱크상체 메쉬
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* TurretMesh;

		//탱크포신 메쉬
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* BarrelMesh;

		//총구위치
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;


	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

		//이동모드 카메라
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* MoveModeCamera;

		//사격모드 카메라
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

	////void SetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent);
	//UFUNCTION(BlueprintCallable, Category = "CustomFunc")
	//	void ToCallSetVehicleMovement(UWheeledVehicleMovementComponent* MovementComponent);


	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//UFUNCTION(BlueprintCallable, Category = "Animation")
//		UWheeledVehicleMovementComponent* 

	
	//사격버튼 눌렀을 때
	void FireStart();
	//사격버튼 떼었을 때
	void FireEnd();
	//시점변경시 visible설정
	void FireMode();
	//인벤토리생성
	void SetDefaultInvetory();
	//탱크에게 데미지를 전달.
	UFUNCTION()
	void ApplyDamage(float damage);
	
	//AP를 반환하는 함수
	float getAP() {
		return AP;
	}

	

	float getPower() { return CurShootingPower; }

	void SetTurn(bool b) { myTurn = b; }

	bool GetTurn() { return myTurn; }

	void ChangeTurn();

	bool PossessTank;


	

private:


	//현재 탄환을 가리키는 포인터,TSubclassOf를 사용하는 이유는 <타입>을 상속하는 클래스만 할당하게 하기 위함.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> CurrentProjectile;
	TArray<UParticleSystemComponent*> BeamArray;

	//빔을 담고있는 배열
	//탄환을 넣을 인벤토리
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AProjectile>> ProjectileInventory;
	//사격모드일 때 Barrel과 Gun 메쉬 변환.
	void ChangeFiremodeBody();

	FVector GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time);

	void DrawTrajectory();

	void ClearBeam();

	void RaisePower();

	//버튼 클릭 시 시점변경
	void ChangeCamera(bool bIsFireMode);

	//UParticleSystemComponent* TrajectoryBeam;
	
	//빔을 그리는 함수
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	void ChangePawn();

	//유도미사일에게 넘겨줄 유도타겟
	class AActor* HomingTarget;


	//사격모드인가?
	bool bIsFireMode;
	//최대체력
	float MaxHealth;
	//최대실드량
	float MaxShield;
	//현재체력
	float CurrentHealth;
	//현재AP
	float AP;

	bool myTurn;

	float RaisingRate;

	float MinShootingPower;

	float MaxShootingPower;

	//발사 파워
	float CurShootingPower;

	//발사중인가?
	bool bIsShoot;

	bool bIsPushFire;


	/////////////////////////////
	// AI관련 
private:

	// 플레이어인지 AI인지 판단하는 bool, true면 플레이어
	bool isNotAI;

	// AI가 포를 쏘기 위한 함수, 파워값을 받아서 FireEnd() 함수로 보냄
	void ShootAI(float);


	void TurnAI();

	// AI의 포신방향으로 나가는 collision box, 터렛을 회전시킬 때 사용
	UBoxComponent* ViewBox;


	int PawnNum;

	// AI가 플레이어의 위치를 추적하기 위한 변수
	FVector EnemyLocation;

	// collision box 초기화 함수
	void SetViewBoxLocation();

public:

	// AI가 포신을 회전시킬 때 ViewBox에 충돌이 되었는지 확인하는 변수
	int TurretAim;

	// ViewBox에 충돌된 Actor를 저장하는 변수
	AActor * CollisionActor;

	// 게임모드에서 ShootAI를 호출하기 위한 함수, 게임모드에서 탱크에게 파워값을 보내줌
	void CommandShoot(float power) { ShootAI(power); }
	void CommandTurn() { TurnAI(); }
	
	// 게임모드에서 이 탱크가 AI인지 아닌지를 설정해주는 함수
	void SetisNotAI(bool isntAI) { isNotAI = isntAI; }

	// !IsNotAI를 반환, true이면 AI
	bool GetisAI() { return !isNotAI; }

	
	int GetPawnNum() { return PawnNum; }

	// Camera를 외부에서 사용하기 위해 반환
	UCameraComponent* ReturnCamera() { return MoveModeCamera; }

	// ViewBox에 충돌되었을 때 호출되는 함수
	UFUNCTION()
		void FindEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ViewBox에서 벗어났을 때 호출되는 함수
	UFUNCTION()
		void LostEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 플레이어의 위치를 적 AI가 추적하기 위해 위치를 받아 저장시키는 함수
	void SetEnemyLocation(FVector location) { EnemyLocation = location; }

	// 입력된 값을 이용해 터렛을 회전
	void RotateTurret(float);

	// ViewBox의 크기를 늘이거나 줄이는 함수
	void ScaleViewBox();

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

	// 현재 발사중인지 아닌지를 게임모드에서 확인하기 위해 값을 리턴
	bool GetbIsShoot() { return bIsShoot; }


	UBoxComponent* GetViewBox() { return ViewBox; }

	// AI가 어느 방향으로 회전해야할지 계산하기 위해 메쉬의 위치와 포신의 forwardVector를 리턴
	FVector ReturnMeshLocation() { return GetMesh()->GetComponentLocation(); }
	FVector ReturnTurretForwardVector() { return TurretMesh->GetForwardVector(); }
};
