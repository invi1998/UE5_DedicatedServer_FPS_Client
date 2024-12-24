// // Copyright invi1998


#include "Player/DSPlayerController.h"

ADSPlayerController::ADSPlayerController()
{
}

void ADSPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (GetNetMode() == NM_Standalone) return;		// 单机模式不需要PingPong

	if (IsLocalController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());
	}
}

// 因为服务器的定时器我们设定是倒计时，也就是说服务器传递的CountdownTimeLeft是倒计时的时间，所以我们需要减去单程时间
void ADSPlayerController::Client_TimerUpdate_Implementation(float CountdownTimeLeft, ECountdownTimerType TimerType)
{
	OnTimerUpdated.Broadcast(CountdownTimeLeft - SingleTripTime, TimerType);
}

void ADSPlayerController::Client_TimerFinished_Implementation(float CountdownTimeLeft, ECountdownTimerType TimerType)
{
	OnTimerFinished.Broadcast(CountdownTimeLeft - SingleTripTime, TimerType);
}

void ADSPlayerController::Client_TimerPaused_Implementation(float CountdownTimeLeft, ECountdownTimerType TimerType)
{
	OnTimerPaused.Broadcast(CountdownTimeLeft - SingleTripTime, TimerType);
}

void ADSPlayerController::Client_TimerResumed_Implementation(float CountdownTimeLeft, ECountdownTimerType TimerType)
{
	OnTimerResumed.Broadcast(CountdownTimeLeft - SingleTripTime, TimerType);
}

void ADSPlayerController::Client_Pong_Implementation(float TimeStamp)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeStamp;
	SingleTripTime = RoundTripTime * 0.5f;
}

void ADSPlayerController::Server_Ping_Implementation(float TimeStamp)
{
	Client_Pong(TimeStamp);
}
