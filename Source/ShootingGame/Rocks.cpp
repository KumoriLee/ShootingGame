// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocks.h"
#include "FrameComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "TiltComponent.h"
#include "EnemyHitComponent.h"


ARocks::ARocks()
{

	// 画面範囲外判定コンポーネント。bAutoDestroy=false を設定し、デリゲートで破棄を制御する
	FrameComp = CreateDefaultSubobject<UFrameComponent>(TEXT("FrameComp"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	EnemyHitComp = CreateDefaultSubobject<UEnemyHitComponent>(TEXT("EnemyHitComp"));

	PrimaryActorTick.bCanEverTick = true;
}

void ARocks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	FRotator CurrentRotation = GetActorRotation();

	// 前方へ移動
	CurrentLocation.X += ForwardSpeed * DeltaTime;
	SetActorLocation(CurrentLocation);

	CurrentRotation.Roll += RotateSpeed * DeltaTime;

	SetActorRotation(CurrentRotation);
}

void ARocks::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
