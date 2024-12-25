// // Copyright invi1998


#include "UI/Timers/TimerWidget.h"

#include "CommonUI/Public/CommonTextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/DSPlayerController.h"

void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwningPlayerController = Cast<ADSPlayerController>(GetOwningPlayer());
	if (IsValid(OwningPlayerController))
	{
		OwningPlayerController->OnTimerUpdated.AddDynamic(this, &UTimerWidget::OnTimerUpdated);
		OwningPlayerController->OnTimerFinished.AddDynamic(this, &UTimerWidget::OnTimerFinished);
	}

	if (bHideOnZero)
	{
		TextBlock_Timer->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bActive)
	{
		UpdateCountdownTime(InternalCountdownTimeLeft - InDeltaTime);
	}
}

void UTimerWidget::OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType InTimerType)
{
	if (InTimerType != TimerType) return;
	if (!bActive)
	{
		TimerStart(CountdownTimeLeft);
	}
	UpdateCountdownTime(CountdownTimeLeft);
	K2_OnTimerUpdated(CountdownTimeLeft, TimerType);
}

void UTimerWidget::OnTimerFinished(float CountdownTimeLeft, ECountdownTimerType InTimerType)
{
	if (InTimerType != TimerType) return;
	TimerStop();
	K2_OnTimerFinished(0.f, TimerType);
}

FString UTimerWidget::FormatTimeAsString(float TimeInSeconds) const
{
	TimeInSeconds = bCanBeNegative ? TimeInSeconds : FMath::Abs(TimeInSeconds);
	FString DisplayTimeString{};
	if (bShowMilliseconds)
	{
		DisplayTimeString = UKismetStringLibrary::TimeSecondsToString(TimeInSeconds);
	}
	else
	{
		const TCHAR* NegativeModifier = TimeInSeconds < 0.f? TEXT("-") : TEXT("");
		TimeInSeconds = FMath::Abs(TimeInSeconds);
	
		const int32 NumMinutes = FMath::FloorToInt(TimeInSeconds/60.f);
		const int32 NumSeconds = FMath::FloorToInt(TimeInSeconds-(NumMinutes*60.f));
		DisplayTimeString = FString::Printf(TEXT("%s%02d:%02d"), NegativeModifier, NumMinutes, NumSeconds);
	}
	return DisplayTimeString;
}

void UTimerWidget::TimerStart(float InitialTime)
{
	bActive = true;
	TextBlock_Timer->SetVisibility(ESlateVisibility::Visible);
}

void UTimerWidget::TimerStop()
{
	bActive = false;
	UpdateCountdownTime(0.f);
	if (bHideOnZero)
	{
		TextBlock_Timer->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTimerWidget::UpdateCountdownTime(float NewTime)
{
	InternalCountdownTimeLeft = NewTime;
	TextBlock_Timer->SetText(FText::FromString(FormatTimeAsString(InternalCountdownTimeLeft)));
}

