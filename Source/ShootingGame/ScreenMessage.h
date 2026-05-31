// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"		// TextBlock 控件：显示静态文字

#include "ScreenMessage.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UScreenMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 绑定到蓝图中的 MessageTextBlock 控件，用于显示倒计时/胜负文字 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MessageTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PointTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimeTextBlock;

	/**
	 * 设置屏幕显示文字
	 * @param Message 要显示的字符串
	 */
	void SetMessageText(FString Message);

	void SetPointText(int32 Point);

	void SetTimeText(int32 Time);

	
};
