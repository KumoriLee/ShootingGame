// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	// 飛行軌跡の尾引きパーティクル（弾にアタッチして持続再生）
	TrialParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialParticles"));
	TrialParticles->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//デリゲート
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	// 生成時にプレイヤーを一度だけ取得し、毎フレームの取得を回避
	CachedPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	// 発射音を再生
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchSound, GetActorLocation());
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 画面の相対範囲から外れたか判定
	if (CachedPlayerPawn && !bIsOutOfBounds)
	{
		float PlayerX = CachedPlayerPawn->GetActorLocation().X;
		float CurrentX = GetActorLocation().X;

		// 弾がプレイヤーより後方、範囲外と判定
		if (CurrentX > PlayerX + 500.0f)
		{
			bIsOutOfBounds = true;
			// エンジン内蔵関数を呼び出し、3秒後にこの弾を自動的に破棄
			SetLifeSpan(3.0f);
		}
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

