// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "Rocks.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	// デフォルトプレイヤーを取得し Raiden にキャスト
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		raiden = Cast<ARaiden>(PlayerPawn);

		if (!raiden)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the player actor!"));
		}
	}

	// プレイヤー操作を無効化（カウントダウン中）
	if (raiden)
	{
		raiden->SetPlayerEnabled(false);
	}

	// UIウィジェットを作成してビューポートに追加
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (ScreenMessageClass && PC)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PC, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToViewport();
			ScreenMessageWidget->SetMessageText(TEXT("3"));
		}
	}

	// カウントダウン開始（3 → 2 → 1 → Go）
	CountdownStep = 3;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, [this]()
	{
		CountdownStep--;

		if (!ScreenMessageWidget) return;

		if (CountdownStep > 0)
		{
			ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownStep));
		}
		else
		{
			// "Go" を表示
			ScreenMessageWidget->SetMessageText(TEXT("Go"));
			GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

			// 0.6秒後に "Go" を消してゲーム開始
			FTimerHandle GoTimer;
			GetWorldTimerManager().SetTimer(GoTimer, [this]()
			{
				if (ScreenMessageWidget)
				{
					ScreenMessageWidget->SetMessageText(TEXT(""));
				}
				StartGame();
			}, 0.6f, false);
		}
	}, 1.0f, true);
}

void AShootingGameMode::StartGame()
{
	// プレイヤー操作を再有効化
	if (raiden)
	{
		raiden->SetPlayerEnabled(true);
	}

	// 統一生成タイマーを開始（0.5秒周期で全 SpawnEntries を巡回）
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AShootingGameMode::SpawnTick, 0.5f, true);

	// ゲーム制限時間タイマーを開始、満了時に EndGame を実行
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AShootingGameMode::EndGame, GameDuration, false);

	// UI の初期表示
	if (ScreenMessageWidget)
	{
		ScreenMessageWidget->SetPointText(point);
		ScreenMessageWidget->SetTimeText(FMath::CeilToInt(GameDuration));
	}

	// 残り時間表示を 0.2 秒ごとに更新
	GetWorldTimerManager().SetTimer(UITimerHandle, [this]()
	{
		if (ScreenMessageWidget)
		{
			float Remaining = GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
			ScreenMessageWidget->SetTimeText(FMath::CeilToInt(Remaining));
		}
	}, 0.2f, true);
}

void AShootingGameMode::EndGame()
{
	// 制限時間経過
	StopEnemy();
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(UITimerHandle);

	if (raiden && raiden->IsAlive)
	{
		raiden->bCanAct = false;
	}

	bGameEnded = true;

	if (ScreenMessageWidget)
	{
		ScreenMessageWidget->SetMessageText(TEXT("Victory"));
	}

	// 1.5 秒後に "Click to Restart" を表示（実際のリスタートは OnRestartInput で処理）
	FTimerHandle PromptTimer;
	GetWorldTimerManager().SetTimer(PromptTimer, [this]()
	{
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText(TEXT("Click to Restart"));
		}
	}, 1.5f, false);
}

void AShootingGameMode::RestartGame()
{
	// 現在のレベルを再読み込みしてゲームをリスタート
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AShootingGameMode::StopEnemy()
{
	// シーン内の全 BasePawn 派生アクターを取得し Tick とタイマーを停止
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePawn::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor && Actor != raiden) // プレイヤーは停止しない
		{
			Actor->SetActorTickEnabled(false);
			GetWorldTimerManager().ClearAllTimersForObject(Actor);
		}
	}
}

void AShootingGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == raiden)
	{
		// プレイヤー死亡
		raiden->HandleDestruction();

		if (bGameEnded) return;

		StopEnemy();
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
		GetWorldTimerManager().ClearTimer(UITimerHandle);

		bGameEnded = true;

		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText(TEXT("GameOver"));
		}

		// 1.5秒後に "Click to Restart" を表示（実際のリスタートは OnRestartInput で処理）
		FTimerHandle PromptTimer;
		GetWorldTimerManager().SetTimer(PromptTimer, [this]()
		{
			if (ScreenMessageWidget)
			{
				ScreenMessageWidget->SetMessageText(TEXT("Click to Restart"));
			}
		}, 1.5f, false);
	}
	else
	{
		// 敵死亡
		AEnemyRaidenPawn* DeadEnemyRaiden = Cast<AEnemyRaidenPawn>(DeadActor);
		if (DeadEnemyRaiden)
		{
			DeadEnemyRaiden->HandleDestruction();
			point++;

			// スコア表示を即時更新
			if (ScreenMessageWidget)
			{
				ScreenMessageWidget->SetPointText(point);
			}
		}
		else
		{
			// 障害物（Rock）の破壊
			ARocks* DeadRock = Cast<ARocks>(DeadActor);
			if (DeadRock)
			{
				DeadRock->HandleDestruction();
			}
		}
	}
}

void AShootingGameMode::SpawnTick()
{
	// プレイヤー死亡時は生成停止
	if (!raiden || !raiden->IsAlive)
	{
		return;
	}

	const float DeltaTime = 0.5f; // タイマー周期

	for (FSpawnEntry& Entry : SpawnEntries)
	{
		if (!Entry.bEnabled || !Entry.ActorClass)
		{
			continue;
		}

		Entry.ElapsedSinceLastSpawn += DeltaTime;
		if (Entry.ElapsedSinceLastSpawn >= Entry.SpawnInterval)
		{
			Entry.ElapsedSinceLastSpawn = 0.0f;
			DoSpawn(Entry);
		}
	}
}

void AShootingGameMode::DoSpawn(const FSpawnEntry& Entry)
{
	FVector PlayerLoc = raiden->GetActorLocation();

	FVector SpawnLocation(
		PlayerLoc.X + Entry.SpawnOffset.X,
		PlayerLoc.Y + Entry.SpawnOffset.Y + FMath::RandRange(-Entry.SpawnRangeY, Entry.SpawnRangeY),
		PlayerLoc.Z + Entry.SpawnOffset.Z
	);

	FRotator SpawnRotation(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor<ABasePawn>(Entry.ActorClass, SpawnLocation, SpawnRotation);
}

float AShootingGameMode::GetStartDelayRemaining() const
{
	// カウントダウン中の現在のステップ値を返す
	return (float)CountdownStep;
}

float AShootingGameMode::GetGameTimeRemaining() const
{
	// ゲームの残り制限時間（秒）を返す
	return GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
}
