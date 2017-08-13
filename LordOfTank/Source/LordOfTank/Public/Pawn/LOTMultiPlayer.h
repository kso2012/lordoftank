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

	//탱크 엔진 사운드
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

	//사격버튼 눌렀을 때
	void FireStart();
	////사격버튼 떼었을 때
	void FireEnd();
	//시점변경시 visible설정
	void FireMode();

	void ChangeTurn() { bIsPushFire = false; bIsShoot = false; }
	////인벤토리생성
	void SetDefaultInvetory();

	void SetArmorPierceNum();

	void SetHomingNum();

	void SetEmpNum();

	void SetTurn(bool turn) { bIsMyTurn = turn; }

	UFUNCTION(BlueprintCallable, Category = "ProjectileNum")
		int ProjectileType() { return CurInventoryIndex; }

	UFUNCTION(BlueprintCallable, Category = "ProjectileNum")
		int ProjectileNum() { return HadProjectileNum[CurInventoryIndex]; }

	//가지고 있는 탄의 개수를 저장할 배열
		int HadProjectileNum[4];

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileNum")
		int CurInventoryIndex;

private:


	////현재 탄환을 가리키는 포인터,TSubclassOf를 사용하는 이유는 <타입>을 상속하는 클래스만 할당하게 하기 위함.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> CurrentProjectile;
	TArray<UParticleSystemComponent*> BeamArray;

	////빔을 담고있는 배열
	////탄환을 넣을 인벤토리
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AProjectile>> ProjectileInventory;

	
	////사격모드일 때 Barrel과 Gun 메쉬 변환.
	void ChangeFiremodeBody();

	void ChangePawn();

	FVector GetSegmentatTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float time);

	void DrawTrajectory();

	void ClearBeam();

	void RaisePower();

	////버튼 클릭 시 시점변경
	void ChangeCamera(bool bIsFireMode);

	////빔을 그리는 함수
	void DrawBeam(FVector StartLocation, FVector EndLocation);

	
	class AActor* HomingTarget;


	//사격모드인가?
	bool bIsFireMode;

	float RaisingRate;

	float MinShootingPower;

	float MaxShootingPower;

	//발사 파워
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
	//발사중인가?
	bool bIsShoot;

	bool bIsPushFire;

	class AProjectile* TempActor;

	void TurnOver();

	float d_time;

	bool bIsMyTurn;



};
