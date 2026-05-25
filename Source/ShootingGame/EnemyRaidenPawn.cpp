// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRaidenPawn.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"

void AEnemyRaidenPawn::BeginPlay()
{
	Super::BeginPlay();

	//プレーヤーポインター
	Raiden = Cast<ARaiden>(UGameplayStatics::GetPlayerPawn(this, 0));

	//カプセルにhitイベントをつける
	if (CapsuleComp)
	{
		CapsuleComp->OnComponentHit.AddDynamic(this, &AEnemyRaidenPawn::OnEnemyHit);
	}

	FTimerHandle FireTimerHandle;

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemyRaidenPawn::CheckFireCondition, FireRate, true);

}

void AEnemyRaidenPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//プレーヤーが死んでるか
	if (Raiden && !Raiden->IsAlive)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	//basic movement
	CurrentLocation.X += ForwardSpeed * DeltaTime;

	// Y軸の追跡および傾きトリガーのロジック
	if (Raiden)
	{
		float DistanceToPlayer = FVector::Dist(CurrentLocation, Raiden->GetActorLocation());
		if (DistanceToPlayer <= SearchRange)
		{
			float TargetY = Raiden->GetActorLocation().Y;

			// Y軸の差を計算し、左右どちらに飛ぶかを判定する
			float DeltaY = TargetY - CurrentLocation.Y;

			// スムーズな移動
			CurrentLocation.Y = FMath::FInterpConstantTo(CurrentLocation.Y, TargetY, DeltaTime, YTargetSpeed);

			// 動的な傾き：目標のY軸との距離が10を超えた場合、傾きをトリガーする
			if (FMath::Abs(DeltaY) > 10.0f)
			{
				// 差の正負に応じて、左に傾くか右に傾くかを決定する
				TargetRoll = (DeltaY > 0) ? MaxRollAngle : -MaxRollAngle;
			}
			else
			{
				TargetRoll = 0.0f; // 追跡位置に到達、姿勢を戻す
			}
		}
		else
		{
			TargetRoll = 0.0f; // 追跡範囲外のため、正面への飛行を維持する
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
	if (Raiden && Raiden->IsAlive && InFireRange() && bIsFiringPhase && !bIsOutOfBounds)
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

void AEnemyRaidenPawn::OnEnemyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//自身ではないの有効なコンフリクト
	if (OtherActor && OtherActor != this)
	{
		ARaiden* HitPlayer = Cast<ARaiden>(OtherActor);
		//プレーヤーがどうか
		if (HitPlayer && HitPlayer->IsAlive)
		{
			UE_LOG(LogTemp, Display, TEXT("Crashed"));

			//敵の最大生命を保存する用の変数
			float EnemyMaxHP = 0.0f;

			//自身のhealthcompを獲得、maxHealthを獲得
			UHealthComponent* EnemyHealthComp = FindComponentByClass<UHealthComponent>();
			if (EnemyHealthComp)
			{
				EnemyMaxHP = EnemyHealthComp->maxHealth;
			}

			//プレーヤーにダメージを与える
			UGameplayStatics::ApplyDamage(HitPlayer, EnemyMaxHP, nullptr, this, UDamageType::StaticClass());

			//自分にmaxhealthを引く--直接に普通の死亡処理する
			UGameplayStatics::ApplyDamage(this, EnemyMaxHP, nullptr, this, UDamageType::StaticClass());
		}
	}
}
