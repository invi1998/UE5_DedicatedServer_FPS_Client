﻿// // Copyright invi1998


#include "Game/DS_MatchGameMode.h"

#include "Player/DSPlayerController.h"

ADS_MatchGameMode::ADS_MatchGameMode()
{
	bUseSeamlessTravel = true;		// 使用无缝旅行
	MatchStatus = EMatchStatus::WaitingForPlayers;
	PreMatchTimer.Type = ECountdownTimerType::PreMatchCountdown;
	MatchTimer.Type = ECountdownTimerType::MatchCountdown;
	PostMatchTimer.Type = ECountdownTimerType::PostMatchCountdown;
}

void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 对于无缝旅行（Seamless Travel）的玩家，登录后是不会被调用PostLogin
	// 所以我们需要在InitSeamlessTravelPlayer中调用CheckAndStartLobbyCountdown
	
	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADS_MatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADS_MatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	RemovePlayerSession(Exiting);
}

void ADS_MatchGameMode::OnCountdownTimerFinished(ECountdownTimerType InTimerType)
{
	Super::OnCountdownTimerFinished(InTimerType);

	if (InTimerType == ECountdownTimerType::PreMatchCountdown)
	{
		StopCountdownTimer(PreMatchTimer);
		MatchStatus = EMatchStatus::InMatch;
		StartCountdownTimer(MatchTimer);
		SetClientInputEnabled(true);
	}
	if (InTimerType == ECountdownTimerType::MatchCountdown)
	{
		StopCountdownTimer(MatchTimer);
		MatchStatus = EMatchStatus::PostMatch;
		StartCountdownTimer(PostMatchTimer);
		SetClientInputEnabled(false);
	}
	if (InTimerType == ECountdownTimerType::PostMatchCountdown)
	{
		StopCountdownTimer(PostMatchTimer);
		MatchStatus = EMatchStatus::SeamlessTravel;
		TrySeamlessTravel(LobbyMap);
	}
}

void ADS_MatchGameMode::SetClientInputEnabled(bool bEnable)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(*It);
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_SetInputEnabled(bEnable);
		}
	}
}