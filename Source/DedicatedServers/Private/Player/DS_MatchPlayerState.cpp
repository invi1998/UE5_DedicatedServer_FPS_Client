// // Copyright invi1998


#include "Player/DS_MatchPlayerState.h"

#include "UI/GameStats/GameStatsManager.h"

void ADS_MatchPlayerState::OnMatchTimerFinished(const FString& UserName)
{
}

void ADS_MatchPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	
	
}

void ADS_MatchPlayerState::RecordMatchStats(const FDSRecordMatchStatsInput& MatchStatsInput) const
{
	if (IsValid(GameStatsManager))
	{
		GameStatsManager->RecordMatchStats(MatchStatsInput);
	}
}
