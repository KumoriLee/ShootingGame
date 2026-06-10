// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "TiltComponent.h"
#include "Kismet/GameplayStatics.h"	// ゲームプレイ静的ユーティリティ：PlaySoundAtLocation など

// デフォルト値の設定
ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// コライダーをルートコンポーネントに設定。後でモデルを自由に差し替え可能にする
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	// 弾のスポーン位置を BaseMesh にアタッチ
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);

	// 傾斜コンポーネントを作成し、傾斜対象メッシュを設定
	TiltComp = CreateDefaultSubobject<UTiltComponent>(TEXT("TiltComp"));
	TiltComp->SetTargetMesh(BaseMesh);
}

void ABasePawn::fire()
{
	//ProjectileSpawnPointの場所で弾を生成
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	if (Projectile)
	{
		Projectile->SetOwner(this);
	}
	
}

void ABasePawn::HandleDestruction()
{
	// 死亡Niagaraパーティクルエフェクトを生成
	if (DeathParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathParticles, GetActorLocation(), GetActorRotation());
	}

	// 爆発サウンドを再生
	if (BoomSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BoomSound, GetActorLocation());
	}

}
