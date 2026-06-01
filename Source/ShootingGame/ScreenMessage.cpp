// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	// FString → FText に変換してバインド先の TextBlock に反映
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
