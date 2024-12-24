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

void ADSPlayerController::Client_Pong_Implementation(float TimeStamp)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeStamp;
	SingleTripTime = RoundTripTime * 0.5f;
}

void ADSPlayerController::Server_Ping_Implementation(float TimeStamp)
{
	Client_Pong(TimeStamp);
}
