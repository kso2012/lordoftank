// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Item.generated.h"

UCLASS()
class LORDOFTANK_API AItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* ParachuteMesh;

	float FallingSpeed;

public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFallingSpeed();

	UBoxComponent* CollBox;

	UFUNCTION()
		void CheckFalling(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bIsFalling;

	bool bUseGravity;

	void CheckLocation();

	float Gravity;

	void ShakeBox();

	bool bShakePlus;

	float ParachuteSize;

	void ScaleUpParachute();

	void ScaleDownParachute();

	bool bIsMoving;

	//아이템 종류를 나타내는 것
	int item_num;

	//아이템의 인덱스
	int item_count;

	bool bRemainItem;
	
};
