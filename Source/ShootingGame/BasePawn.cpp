// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//コライダーをルートコンポーネントに設定します。これにより、後でモデルを自由に変更できるようになる
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	//生成点をbasemeshにつける
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//傾斜処理
	if (BaseMesh)
	{
		//傾斜できない場合は角度を０にする--異常状況でずっと傾斜して戻れないことを防ぐため
		float RealTargetRoll = bCanRoll ? TargetRoll : 0.0f;

		FRotator CurrentMeshRotation = BaseMesh->GetRelativeRotation();

		//数値を滑らかに補正
		float NewRoll = FMath::FInterpTo(CurrentMeshRotation.Roll, RealTargetRoll, DeltaTime, RollInterpSpeed);

		// 機体の回転を更新（Roll のみ変更）
		BaseMesh->SetRelativeRotation(FRotator(CurrentMeshRotation.Pitch, CurrentMeshRotation.Yaw, NewRoll));
	}
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
}
