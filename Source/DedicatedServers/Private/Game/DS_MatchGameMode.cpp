// // Copyright invi1998


#include "Game/DS_MatchGameMode.h"

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

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADS_MatchGameMode::OnCountdownTimerFinished(ECountdownTimerType InTimerType)
{
	Super::OnCountdownTimerFinished(InTimerType);

	if (InTimerType == ECountdownTimerType::PreMatchCountdown)
	{
		MatchStatus = EMatchStatus::InMatch;
		StartCountdownTimer(MatchTimer);
	}
	if (InTimerType == ECountdownTimerType::MatchCountdown)
	{
		MatchStatus = EMatchStatus::PostMatch;
		StartCountdownTimer(PostMatchTimer);
	}
	if (InTimerType == ECountdownTimerType::PostMatchCountdown)
	{
		MatchStatus = EMatchStatus::SeamlessTravel;
	}
}
