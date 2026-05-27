// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h" // タイマーマネージャーを使用するために必要


void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	// GameModeを使用してデフォルトプレイヤーを設定
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		raiden = Cast<ARaiden>(PlayerPawn);

		if (!raiden)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the player actor!"));
		}
	}

	// 1. ゲーム開始時は準備フェーズとして、プレイヤーの移動と射撃を無効化
	if (raiden)
	{
		raiden->SetPlayerEnabled(false);
	}

	// 2. 開始前3秒のカウントダウンを開始、満了時にStartGameを実行
	GetWorldTimerManager().SetTimer(StartDelayTimerHandle, this, &AShootingGameMode::StartGame, StartDelay, false);
}

void AShootingGameMode::StartGame()
{
	// カウントダウン終了、ゲーム本開始

	// 1. プレイヤーの操作を再有効化
	if (raiden)
	{
		raiden->SetPlayerEnabled(true);
	}

	// 2. 敵の出現を開始
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AShootingGameMode::SpawnEnemy, SpawnInterval, true);

	// 3. 30秒のゲーム制限時間を開始、満了時にEndGameを実行
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AShootingGameMode::EndGame, GameDuration, false);
}

void AShootingGameMode::EndGame()
{
	// 制限時間経過！
	StopEnemy();
	// 1. 敵の出現を停止
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	// 2. プレイヤー操作を無効化し、「時間切れで動けない」状態を表現
	if (raiden && raiden->IsAlive)
	{
		raiden->SetPlayerEnabled(false);
	}

	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AShootingGameMode::RestartGame, RestartDelay, false);

	// ゲームオーバー/勝利UIを表示するロジックをここに追加可能
	UE_LOG(LogTemp, Warning, TEXT("Time is up! Game Over!"));
}

void AShootingGameMode::RestartGame()
{
	// 現在のレベルを再読み込みしてゲームをリスタート
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AShootingGameMode::StopEnemy()
{
	// シーン内の全 EnemyRaidenPawn を取得
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyRaidenPawn::StaticClass(), FoundEnemies);

	for (AActor* Enemy : FoundEnemies)
	{
		if (Enemy)
		{
			Enemy->SetActorTickEnabled(false);
		}
	}
}

void AShootingGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == raiden)
	{
		// プレイヤー死亡
		raiden->HandleDestruction();
		IsGameOver = true;

		StopEnemy();

		// 1. 敵の出現を停止
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

		// 2. メインの制限時間カウントダウンも停止（ゲームが早期終了したため）
		GetWorldTimerManager().ClearTimer(GameTimerHandle);

		// 3. 死亡後リスタートカウントダウン開始、満了時にRestartGameを実行
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AShootingGameMode::RestartGame, RestartDelay, false);
	}
	else
	{
		// 敵死亡
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
	// プレイヤー死亡直後に敵が生成されるのを防ぐための生存チェック
	if (EnemyClassToSpawn && raiden && raiden->IsAlive)
	{
		FVector PlayerLoc = raiden->GetActorLocation();

		FVector SpawnLocation(PlayerLoc.X - 4000.0f, PlayerLoc.Y + FMath::RandRange(-2000.0f, 2000.0f), PlayerLoc.Z);

		FRotator SpawnRotation(0.0f, 0.0f, 0.0f);

		GetWorld()->SpawnActor<AEnemyRaidenPawn>(EnemyClassToSpawn, SpawnLocation, SpawnRotation);
	}
}

float AShootingGameMode::GetStartDelayRemaining() const
{
	// ゲーム開始までの残り時間（秒）を返す
	return GetWorldTimerManager().GetTimerRemaining(StartDelayTimerHandle);
}

float AShootingGameMode::GetGameTimeRemaining() const
{
	// ゲームの残り制限時間（秒）を返す
	return GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
}