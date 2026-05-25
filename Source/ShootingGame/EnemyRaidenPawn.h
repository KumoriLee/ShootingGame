// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Raiden.h"

#include "EnemyRaidenPawn.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AEnemyRaidenPawn : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 1500.0f; 
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.5f; 

	//動きに関する変数
	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float YTargetSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SearchRange = 2000.0f;


	ARaiden* Raiden;

	bool InFireRange();

	void CheckFireCondition();
	void HandleDestruction();

	//プレーヤーに衝突の場合
	UFUNCTION()
	void OnEnemyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	float FireCycleTimer = 0.0f;
	bool bIsFiringPhase = true;
	bool bIsOutOfBounds = false;

};
