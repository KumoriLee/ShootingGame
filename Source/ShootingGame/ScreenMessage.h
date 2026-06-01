// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"		// TextBlock ウィジェット：静的テキストを表示

#include "ScreenMessage.generated.h"

/**
 * 画面メッセージ UI ウィジェット
 * カウントダウン / 勝敗メッセージ / スコア / 残り時間を表示する
 */
UCLASS()
class SHOOTINGGAME_API UScreenMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	/** ブループリントの MessageTextBlock にバインドされるテキストブロック。カウントダウン / 勝敗メッセージを表示 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MessageTextBlock;

	/** ブループリントの PointTextBlock にバインドされるテキストブロック。スコアを表示 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PointTextBlock;

	/** ブループリントの TimeTextBlock にバインドされるテキストブロック。残り時間を表示 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimeTextBlock;

	/**
	 * 画面中央のメッセージテキストを設定する
	 * @param Message 表示する文字列
	 */
	void SetMessageText(FString Message);

	/**
	 * スコア表示を更新する
	 * @param Point 現在のスコア
	 */
	void SetPointText(int32 Point);

	/**
	 * 残り時間表示を更新する
	 * @param Time 残り秒数
	 */
	void SetTimeText(int32 Time);

	
};
