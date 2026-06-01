// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "FrameComponent.h"
#include "Kismet/GameplayStatics.h"


// デフォルト値の設定
AProjectile::AProjectile()
{
	// Tick() を毎フレーム呼び出すよう設定。弾は移動コンポーネント任せなので無効
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	// 飛行軌跡の尾引きパーティクル（弾にアタッチして持続再生）
	TrialParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialParticles"));
	TrialParticles->SetupAttachment(RootComponent);

	// 画面範囲外判定コンポーネント。bAutoDestroy=true（既定）により、範囲外で自動 SetLifeSpan
	FrameComp = CreateDefaultSubobject<UFrameComponent>(TEXT("FrameComp"));
}

// ゲーム開始時またはスポーン時に呼ばれる
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	// ヒットデリゲートをバインド
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	// 発射音を再生
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchSound, GetActorLocation());
	}
}



void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// 自身・Owner 以外のアクターにのみダメージを与える（自傷防止）
		if (OtherActor && (OtherActor != MyOwner) && (OtherActor != this))
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				Damage,
				MyOwner->GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
		}

		// 命中パーティクルエフェクトを生成
		if (HitParticles)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, GetActorLocation(), GetActorRotation());
		}

		// 命中音を再生
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		}
	}
	Destroy();
}

