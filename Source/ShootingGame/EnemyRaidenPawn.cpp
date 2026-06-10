// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRaidenPawn.h"
#include "FrameComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "TiltComponent.h"


AEnemyRaidenPawn::AEnemyRaidenPawn()
{
	// 画面範囲外判定コンポーネント。bAutoDestroy=false に設定し、範囲外時の破棄は委任で自前処理
	FrameComp = CreateDefaultSubobject<UFrameComponent>(TEXT("FrameComp"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HtalthComp"));

	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyRaidenPawn::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤー機へのポインタをキャッシュ
	Raiden = Cast<ARaiden>(UGameplayStatics::GetPlayerPawn(this, 0));

	// カプセルコンポーネントに衝突イベントをバインド
	if (CapsuleComp)
	{
		CapsuleComp->OnComponentHit.AddDynamic(this, &AEnemyRaidenPawn::OnEnemyHit);
	}

	// 射撃条件チェック用の反復タイマーを開始
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemyRaidenPawn::CheckFireCondition, FireRate, true);

}

void AEnemyRaidenPawn::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	// プレイヤーが死亡済みなら更新を停止
	if (Raiden && !Raiden->IsAlive)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	// 基本前進移動
	CurrentLocation.X += ForwardSpeed * DeltaTime;

	// Y 軸の追跡および傾斜トリガーのロジック
	if (Raiden)
	{
		float DistanceToPlayer = FVector::Dist(CurrentLocation, Raiden->GetActorLocation());
		if (DistanceToPlayer <= SearchRange)
		{
			float TargetY = Raiden->GetActorLocation().Y;

			// Y 軸の差を計算し、左右どちらに寄せるか判定
			float DeltaY = TargetY - CurrentLocation.Y;

			// 目標 Y 座標へスムーズに移動
			CurrentLocation.Y = FMath::FInterpConstantTo(CurrentLocation.Y, TargetY, DeltaTime, YTargetSpeed);

			// 動的傾斜：目標との Y 軸差が 10 を超えたら傾斜をトリガー
			if (FMath::Abs(DeltaY) > 10.0f)
			{
				// 差の正負に応じて左右どちらに傾くか決定
				if (TiltComp)
				{
					TiltComp->SetTargetRoll((DeltaY > 0) ? TiltComp->MaxRollAngle : -TiltComp->MaxRollAngle);
				}
			}
			else
			{
				if (TiltComp)
				{
					TiltComp->ResetRoll(); // 追跡位置に到達、角度を戻す
				}
			}
		}
		else
		{
			if (TiltComp)
			{
				TiltComp->ResetRoll(); // 追跡範囲外のため正面飛行を維持
			}
		}
	}
	SetActorLocation(CurrentLocation);

	// 射撃サイクルのステートマシン
	FireCycleTimer += DeltaTime;
	if (bIsFiringPhase && FireCycleTimer >= 5.0f) // 発射継続時間
	{
		bIsFiringPhase = false;
		FireCycleTimer = 0.0f;
	}
	else if (!bIsFiringPhase && FireCycleTimer >= 2.0f) // クールダウン時間
	{
		bIsFiringPhase = true;
		FireCycleTimer = 0.0f;
	}
}

void AEnemyRaidenPawn::CheckFireCondition()
{
	// 全条件を満たせば射撃実行（生存・射程内・発射フェーズ・画面内）
	if (Raiden && Raiden->IsAlive && InFireRange() && bIsFiringPhase && !FrameComp->IsOutOfBounds())
	{
		fire();
	}
}

bool AEnemyRaidenPawn::InFireRange()
{
	if (Raiden)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), Raiden->GetActorLocation());
		if (DistanceToPlayer <= FireRange)
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
	// 自身以外との有効な衝突のみ処理
	if (OtherActor && OtherActor != this)
	{
		ARaiden* HitPlayer = Cast<ARaiden>(OtherActor);
		// 衝突相手がプレイヤーかどうか確認
		if (HitPlayer && HitPlayer->IsAlive)
		{
			UE_LOG(LogTemp, Display, TEXT("Crashed"));

			// 敵の最大HPを保持する変数
			float EnemyMaxHP = 0.0f;

			// 自身の HealthComp から最大HPを取得
			UHealthComponent* EnemyHealthComp = FindComponentByClass<UHealthComponent>();
			if (EnemyHealthComp)
			{
				EnemyMaxHP = EnemyHealthComp->maxHealth;
			}

			// プレイヤーにダメージを与える
			UGameplayStatics::ApplyDamage(HitPlayer, EnemyMaxHP, nullptr, this, UDamageType::StaticClass());

			// 自分にも最大HP分のダメージを与えて死亡処理を実行
			UGameplayStatics::ApplyDamage(this, EnemyMaxHP, nullptr, this, UDamageType::StaticClass());
		}
	}
}
