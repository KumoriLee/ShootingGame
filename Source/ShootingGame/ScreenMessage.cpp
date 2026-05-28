// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	// 将 FString 转为 FText 后设置到绑定的 TextBlock 控件
	FText MessageText = FText::FromString(Message);
	MessageTextBlock->SetText(MessageText);
}

void UScreenMessage::SetPointText(int32 Point)
{
	FText PointText = FText::AsNumber(Point);
	PointTextBlock->SetText(PointText);
}

void UScreenMessage::SetTimeText(int32 Time)
{
	FText TimeText = FText::AsNumber(Time);
	TimeTextBlock->SetText(TimeText);
}
