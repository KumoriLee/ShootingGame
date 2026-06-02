# ShootingGame

Unreal Engine 5.7 で制作中の横スクロールシューティングゲームです。

## ゲームルール

- **制限時間**：30 秒
- **目的**：時間内に敵機を撃ち落とし、できるだけ多くのスコアを獲得する
- **勝利条件**：制限時間までプレイヤーが生き残る → **Victory**
- **敗北条件**：プレイヤーの HP が 0 になる → **GameOver**
- ゲーム終了後は左クリックでリスタートできます

## 操作方法

| 操作 | 入力 |
|------|------|
| 移動 | 矢印キー（上下左右） |
| 射撃（連射） | F キー（押しっぱなし可） |
| リスタート | 左クリック（ゲーム終了後） |
| 終了 | Escape キー |

## プログラム構成

```
APawn
 └── ABasePawn（共通：カプセルコリジョン、機体メッシュ、傾斜、発射処理）
      ├── ARaiden（プレイヤー機：EnhancedInput、カメラ制御、生存管理）
      └── AEnemyRaidenPawn（敵機：プレイヤー追跡AI、射撃サイクルFSM）

AActor
 ├── AProjectile（弾：移動コンポーネント、命中時にダメージ付与）
 └── ABackground（背景：動的マテリアルによるスクロール）

UActorComponent
 ├── UHealthComponent（体力管理：ダメージ受領 → 0 以下で GameMode に通知）
 └── UFrameComponent（画面範囲外判定：プレイヤーからの距離で自動破棄）

AGameModeBase
 └── AShootingGameMode（ゲームフロー：カウントダウン → StartGame → ActorDied/EndGame）

UUserWidget
 └── UScreenMessage（UI：メッセージ、スコア、残り時間の表示）
```

### ゲームフロー

```
BeginPlay → 3-2-1-Go カウントダウン → StartGame（敵スポーン開始、入力有効化）
  → 制限時間切れ → EndGame → Victory → Click to Restart
  → プレイヤー死亡 → ActorDied → GameOver → Click to Restart
  → 左クリック → RestartGame（レベル再読み込み）
```
