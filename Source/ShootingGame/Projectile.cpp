// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "FrameComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//デリゲート
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

