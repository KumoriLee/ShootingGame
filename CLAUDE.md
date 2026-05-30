# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

##默认对话用中文
##只有在代码里写注释的时候用日文


## Build

```
"C:/Program Files/Epic Games/UE_5.7/Engine/Build/BatchFiles/Build.bat" ShootingGame Win64 Development -Project="C:/unreal/ShootingGame/ShootingGame/ShootingGame.uproject" -WaitMutex -FromMsBuild
```

UE 5.7, single Runtime module `ShootingGame`. Dependencies: Core, CoreUObject, Engine, InputCore, EnhancedInput, UMG, Niagara.

## Architecture

### Class hierarchy

```
APawn
 └── ABasePawn          — 共通：CapsuleComp, BaseMesh, ProjectileSpawnPoint, fire(), HandleDestruction(), 傾斜
      ├── ARaiden       — プレイヤー機：EnhancedInput, SpringArm/Camera, SetPlayerEnabled()
      └── AEnemyRaidenPawn — 敵機：AI追跡, 射撃サイクルFSM, 衝突ダメージ
```

```
AActor
 └── AProjectile       — 弾：UProjectileMovementComponent, 命中パーティクル/音, ApplyDamage
```

```
UActorComponent
 └── UHealthComponent  — 体力：OnTakeAnyDamage → health<=0 で GameMode::ActorDied()
```

```
AGameModeBase
 └── AShootingGameMode  — ゲームフロー：カウントダウン→StartGame→EndGame/ActorDied→クリックリスタート
```

```
UUserWidget
 └── UScreenMessage    — UI：MessageTextBlock / PointTextBlock / TimeTextBlock
```

```
UGameInstance
 └── UShootingGameInstance  — レベル遷移

UGameInstanceSubsystem
 └── ULevelManagerSubsystem — データ駆動レベル管理 (OpenLevel / OpenNextLevel / RestartCurrentLevel)
```

### 死亡フロー

```
Projectile::OnHit() / Enemy::OnEnemyHit()
  → UGameplayStatics::ApplyDamage()
    → UHealthComponent::OnDamageTaken()
      → health <= 0 → ShootingGameMode::ActorDied()
        → プレイヤー: HandleDestruction() → IsAlive=false, GameOver表示
        → 敵: HandleDestruction() → Destroy(), point++ スコア更新
```

### ゲーム状態遷移 (ShootingGameMode)

```
BeginPlay → 3-2-1-Go カウントダウン → StartGame → [プレイ中]
  → GameTimer切れ → EndGame → "Victory" → "Click to Restart"
  → プレイヤー死亡 → ActorDied → "GameOver" → "Click to Restart"
  → IA_Restart (左クリック) → RestartGame() → OpenLevelで再読み込み
```

### 入力 (Enhanced Input)

- `ARaiden` の `SetupPlayerInputComponent` で MoveAction / FireAction / RestartAction をバインド
- カウントダウン中は `SetPlayerEnabled(false)` で全入力を無効化
- ゲーム終了後は `bCanAct=false` で移動/射撃だけ止め、IA_Restart は通す
- IMC は BP_ShootingGameController で設定

