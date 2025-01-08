// // Copyright invi1998


#include "UI/APITest/APITestRecordMatchStatsOverlay.h"

#include "Components/Button.h"
#include "HTTP/HTTPRequestTypes.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/GameStats/GameStatsManager.h"

void UAPITestRecordMatchStatsOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	Button_RecordMatchStats->OnClicked.AddDynamic(this, &UAPITestRecordMatchStatsOverlay::RecordMatchStatsButtonClicked);
	
}

void UAPITestRecordMatchStatsOverlay::RecordMatchStatsButtonClicked()
{
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetOwningLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>();
	if (IsValid(LocalPlayerSubsystem))
	{
		FDSRecordMatchStatsInput MatchStatsInput;
		MatchStatsInput.userName = LocalPlayerSubsystem->UserName;

		FDSMatchStats MatchStats;

		MatchStats.defeats = FMath::RandRange(0, 10);
		MatchStats.hits = FMath::RandRange(0, 10);
		MatchStats.misses = FMath::RandRange(0, 10);
		MatchStats.dethroneElims = FMath::RandRange(0, 10);
		MatchStats.highestStreak = FMath::RandRange(0, 10);
		MatchStats.matchLosses = FMath::RandRange(0, 10);
		MatchStats.matchWins = FMath::RandRange(0, 10);
		MatchStats.revengeElims = FMath::RandRange(0, 10);
		MatchStats.scoredElims = FMath::RandRange(0, 10);
		MatchStats.gotFirstBlood = FMath::RandBool();
		MatchStats.headShotElims = FMath::RandRange(0, 10);
		MatchStats.showStopperElims = FMath::RandRange(0, 10);
		
		MatchStatsInput.matchStats = MatchStats;

		if (IsValid(GameStatsManager))
		{
			GameStatsManager->RecordMatchStats(MatchStatsInput);
		}
	}
}
