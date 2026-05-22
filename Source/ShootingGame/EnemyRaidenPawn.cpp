// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRaidenPawn.h"
#include "Kismet/GameplayStatics.h"

void AEnemyRaidenPawn::BeginPlay()
{
	Super::BeginPlay();

	//プレーヤーポインター
	Raiden = Cast<ARaiden>(UGameplayStatics::GetPlayerPawn(this, 0));

	FTimerHandle FireTimerHandle;

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemyRaidenPawn::CheckFireCondition, FireRate, true);
}

void AEnemyRaidenPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	//basic movement
	CurrentLocation.X += ForwardSpeed * DeltaTime;

	// Y軸追跡ロジック
	if (Raiden)
	{
		float DistanceToPlayer = FVector::Dist(CurrentLocation, Raiden->GetActorLocation());
		if (DistanceToPlayer <= SearchRange)
		{
			// プレイヤーのY座標を滑らかに追跡
			float TargetY = Raiden->GetActorLocation().Y;
			CurrentLocation.Y = FMath::FInterpConstantTo(CurrentLocation.Y, TargetY, DeltaTime, YTargetSpeed);
		}
	}
	SetActorLocation(CurrentLocation);

	// 発射サイクルのステートマシン (5秒発射、2秒停止)
	FireCycleTimer += DeltaTime;
	if (bIsFiringPhase && FireCycleTimer >= 5.0f)
	{
		bIsFiringPhase = false;
		FireCycleTimer = 0.0f;
	}
	else if (!bIsFiringPhase && FireCycleTimer >= 2.0f)
	{
		bIsFiringPhase = true;
		FireCycleTimer = 0.0f;
	}

	// カメラ範囲外への移動判定
	if (Raiden && !bIsOutOfBounds)
	{
		float PlayerX = Raiden->GetActorLocation().X;

		// 敵機のX座標が「プレイヤーのX座標 - 1500」より小さい場合
		if (CurrentLocation.X > PlayerX + 500.0f)
		{
			bIsOutOfBounds = true;
			bIsFiringPhase = false; // 射撃を停止

			// 3秒後に自爆させるタイマーを開始
			FTimerHandle DestroyTimerHandle;
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyRaidenPawn::HandleDestruction, 3.0f, false);
		}
	}
}

void AEnemyRaidenPawn::CheckFireCondition()
{
	if (Raiden && InFireRange() && bIsFiringPhase && !bIsOutOfBounds)
	{
		fire();
	}
}

bool AEnemyRaidenPawn::InFireRange()
{
	if (Raiden)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Raiden->GetActorLocation());
		if (DistanceToTank <= FireRange)
		{
			return true;
		}
	}
	return false;
}

void AEnemyRaidenPawn::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}
