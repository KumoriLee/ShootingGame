// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"


void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	raiden = Cast<ARaiden>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AShootingGameMode::SpawnEnemy, SpawnInterval, true);

	//gamemodeを使用してデフォルトプレイヤーを設定する
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		raiden = Cast<ARaiden>(PlayerPawn);
		if (!raiden)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the player actor!"));
		}
	}

}

void AShootingGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == raiden)
	{
		//tank just died
		raiden->HandleDestruction();
		IsGameOver = true;
	}
	else
	{
		//enemy just died
		AEnemyRaidenPawn* DeadEnemyRaiden = Cast<AEnemyRaidenPawn>(DeadActor);
		if (DeadEnemyRaiden)
		{
			DeadEnemyRaiden->HandleDestruction();
			point++;
		}
		
	}
}

void AShootingGameMode::SpawnEnemy()
{
	if (EnemyClassToSpawn && raiden)
	{
		FVector PlayerLoc = raiden->GetActorLocation();

		FVector SpawnLocation(PlayerLoc.X - 4000.0f, PlayerLoc.Y + FMath::RandRange(-2000.0f, 2000.0f), PlayerLoc.Z);

		FRotator SpawnRotation(0.0f, 0.0f, 0.0f);

		GetWorld()->SpawnActor<AEnemyRaidenPawn>(EnemyClassToSpawn, SpawnLocation, SpawnRotation);
	}
}
