// // Copyright invi1998


#include "Game/DS_GameModeBase.h"

#include "Player/DSPlayerController.h"

void ADS_GameModeBase::StartCountdownTimer(FCountdownTimerHandle& TimerHandle)
{
	TimerHandle.TimerFinishedDelegate.BindWeakLambda(this, [&]()
	{
		StopCountdownTimer(TimerHandle);
		OnCountdownTimerFinished(TimerHandle.Type);
	});

	GetWorldTimerManager().SetTimer(TimerHandle.TimerFinishedHandle, TimerHandle.TimerFinishedDelegate, TimerHandle.Duration, false);

	TimerHandle.UpdateTimerDelegate.BindWeakLambda(this, [&]()
	{
		UpdateCountdownTimer(TimerHandle);
	});

	GetWorldTimerManager().SetTimer(TimerHandle.UpdateTimerHandle, TimerHandle.UpdateTimerDelegate, TimerHandle.UpdateInterval, true);

	UpdateCountdownTimer(TimerHandle);
}

void ADS_GameModeBase::PauseCountdownTimer(FCountdownTimerHandle& TimerHandle)
{
	TimerHandle.State = ECountdownTimerState::Paused;
	// 暂停定时器
	if (TimerHandle.TimerFinishedHandle.IsValid())
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle.TimerFinishedHandle);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(*It);
		if (IsValid(DSPlayerController))
		{
			const float CountdownTimeLeft = TimerHandle.Duration - GetWorldTimerManager().GetTimerElapsed(TimerHandle.TimerFinishedHandle);
			DSPlayerController->Client_TimerPaused(CountdownTimeLeft, TimerHandle.Type);
		}
	}
}

void ADS_GameModeBase::ResumeCountdownTimer(FCountdownTimerHandle& TimerHandle)
{
	TimerHandle.State = ECountdownTimerState::Started;
	// 恢复定时器
	if (TimerHandle.TimerFinishedHandle.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle.TimerFinishedHandle);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(*It);
		if (IsValid(DSPlayerController))
		{
			const float CountdownTimeLeft = TimerHandle.Duration - GetWorldTimerManager().GetTimerElapsed(TimerHandle.TimerFinishedHandle);
			DSPlayerController->Client_TimerResumed(CountdownTimeLeft, TimerHandle.Type);
		}
	}
}

void ADS_GameModeBase::StopCountdownTimer(FCountdownTimerHandle& TimerHandle)
{
	TimerHandle.State = ECountdownTimerState::Finished;
	// 清除定时器
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle.TimerFinishedHandle);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle.UpdateTimerHandle);

	// 检查定时器委托是否绑定，如果绑定则解绑
	if (TimerHandle.TimerFinishedDelegate.IsBound())
	{
		TimerHandle.TimerFinishedDelegate.Unbind();
	}
	if (TimerHandle.UpdateTimerDelegate.IsBound())
	{
		TimerHandle.UpdateTimerDelegate.Unbind();
	}

	// 遍历当前连接到服务器的所有客户端，通知定时器结束
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(*It);
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_TimerFinished(0.0f, TimerHandle.Type);
		}
	}
	
}

void ADS_GameModeBase::UpdateCountdownTimer(const FCountdownTimerHandle& TimerHandle)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(*It);
		if (IsValid(DSPlayerController))
		{
			const float CountdownTimeLeft = TimerHandle.Duration - GetWorldTimerManager().GetTimerElapsed(TimerHandle.TimerFinishedHandle);
			DSPlayerController->Client_TimerUpdate(CountdownTimeLeft, TimerHandle.Type);
		}
	}
}

void ADS_GameModeBase::OnCountdownTimerFinished(ECountdownTimerType InTimerType)
{
}
