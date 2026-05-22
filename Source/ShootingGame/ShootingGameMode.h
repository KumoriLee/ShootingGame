// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Raiden.h"
#include "EnemyRaidenPawn.h"
#include "ShootingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ARaiden* raiden;
	int32 enemyCount;
	int32 point = 0;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemyRaidenPawn> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 3.0f;

	void ActorDied(AActor* DeadActor);

private:
	FTimerHandle SpawnTimerHandle;
	void SpawnEnemy();
};
