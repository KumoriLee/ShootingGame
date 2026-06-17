// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyHitComponent.h"
#include "Raiden.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

UEnemyHitComponent::UEnemyHitComponent()
{
	// このコンポーネントは Tick 不要
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyHitComponent::BeginPlay()
{
	Super::BeginPlay();

	// オーナーの CapsuleComp に衝突イベントをバインド
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UCapsuleComponent* CapsuleComp = Owner->FindComponentByClass<UCapsuleComponent>();
		if (CapsuleComp)
		{
			CapsuleComp->OnComponentHit.AddDynamic(this, &UEnemyHitComponent::OnEnemyHit);
		}
	}
}

void UEnemyHitComponent::OnEnemyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 自身以外との有効な衝突のみ処理
	if (OtherActor && OtherActor != GetOwner())
	{
		ARaiden* HitPlayer = Cast<ARaiden>(OtherActor);
		// 衝突相手がプレイヤーかどうか確認
		if (HitPlayer && HitPlayer->IsAlive)
		{
			UE_LOG(LogTemp, Display, TEXT("Crashed"));

			// 敵とプレイヤーの最大HPを取得し、小さい方を衝突ダメージとする
			float EnemyMaxHP = 0.0f;
			UHealthComponent* EnemyHealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
			if (EnemyHealthComp)
			{
				EnemyMaxHP = EnemyHealthComp->maxHealth;
			}

			float PlayerMaxHP = 0.0f;
			UHealthComponent* PlayerHealthComp = HitPlayer->FindComponentByClass<UHealthComponent>();
			if (PlayerHealthComp)
			{
				PlayerMaxHP = PlayerHealthComp->maxHealth;
			}

			float CollisionDamage = FMath::Min(EnemyMaxHP, PlayerMaxHP);

			// プレイヤーにダメージを与える
			UGameplayStatics::ApplyDamage(HitPlayer, CollisionDamage, nullptr, GetOwner(), UDamageType::StaticClass());

			// 自分にも同じダメージを与えて死亡処理を実行
			UGameplayStatics::ApplyDamage(GetOwner(), CollisionDamage, nullptr, GetOwner(), UDamageType::StaticClass());
		}
	}
}
