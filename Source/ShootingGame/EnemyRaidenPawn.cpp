// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRaidenPawn.h"
#include "FrameComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"


AEnemyRaidenPawn::AEnemyRaidenPawn()
{
	// 画面範囲外判定コンポーネント。bAutoDestroy=false に設定し、範囲外時の破棄は委任で自前処理
	FrameComp = CreateDefaultSubobject<UFrameComponent>(TEXT("FrameComp"));
}

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
				TargetRoll = 0.0f; // 追跡位置に到達、角度戻す
			}
		}
		else
		{
			TargetRoll = 0.0f; // 追跡範囲外のため、正面への飛行を維持する
		}
	}
	SetActorLocation(CurrentLocation);

	// 発射サイクルのステートマシン
	FireCycleTimer += DeltaTime;
	if (bIsFiringPhase && FireCycleTimer >= 5.0f)//発射つつける時間
	{
		bIsFiringPhase = false;
		FireCycleTimer = 0.0f;
	}
	else if (!bIsFiringPhase && FireCycleTimer >= 2.0f)//クールダウン時間
	{
		bIsFiringPhase = true;
		FireCycleTimer = 0.0f;
	}
}

void AEnemyRaidenPawn::CheckFireCondition()
{
	

	// 範囲外判定は FrameComp に問い合わせる（旧 bIsOutOfBounds フラグを置き換え）
	if (Raiden && Raiden->IsAlive && InFireRange() && bIsFiringPhase && !FrameComp->IsOutOfBounds())
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
