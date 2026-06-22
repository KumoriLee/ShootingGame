// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"


// このコンポーネントのデフォルト値を設定
UHealthComponent::UHealthComponent()
{
	// Tick() を毎フレーム呼び出すよう設定。不要な場合は無効にしてパフォーマンスを向上可能
	PrimaryComponentTick.bCanEverTick = false;
}


// ゲーム開始時に呼ばれる
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		ShootingGameMode = Cast<AShootingGameMode>(GameMode);
	}
}



void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.0f)
	{
		health -= Damage;
		// HP が 0 以下になったら GameMode に死亡を通知
		if (health <= 0.0f)
		{
			if (ShootingGameMode)
			{
				ShootingGameMode->ActorDied(DamagedActor);
			}
		}
	}
}

