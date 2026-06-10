// Fill out your copyright notice in the Description page of Project Settings.


#include "TiltComponent.h"


UTiltComponent::UTiltComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTiltComponent::SetTargetRoll(float NewTarget)
{
	TargetRoll = FMath::Clamp(NewTarget, -MaxRollAngle, MaxRollAngle);
}

void UTiltComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTiltComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetMesh)
	{
		return;
	}

	// 傾斜が無効な場合は TargetRoll を 0 に強制 — 異常状態で傾きっぱなしになるのを防止
	float RealTargetRoll = bCanRoll ? TargetRoll : 0.0f;

	FRotator CurrentMeshRotation = TargetMesh->GetRelativeRotation();

	// 現在の角度から目標角度へ滑らかに補間
	float NewRoll = FMath::FInterpTo(CurrentMeshRotation.Roll, RealTargetRoll, DeltaTime, RollInterpSpeed);

	// メッシュの回転を更新（Roll のみ変更）
	TargetMesh->SetRelativeRotation(FRotator(CurrentMeshRotation.Pitch, CurrentMeshRotation.Yaw, NewRoll));
}
