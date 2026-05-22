// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRaidenPawn.h"

void AEnemyRaidenPawn::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemyRaidenPawn::CheckFireCondition, FireRate, true);
}

void AEnemyRaidenPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange())
	{

	}
}

void AEnemyRaidenPawn::CheckFireCondition()
{
	if (Raiden && InFireRange())
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
