// // Copyright invi1998


#include "UI/portal/Dashboard/Leaderboard.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HTTP/HTTPRequestTypes.h"
#include "UI/portal/Dashboard/LeaderboardCard.h"

void ULeaderboard::PopulateLeaderboard(const TArray<FDSLeaderboardItem>& LeaderboardItems)
{
	ScrollBox_Leaderboard->ClearChildren();

	for (const FDSLeaderboardItem& LeaderboardItem : LeaderboardItems)
	{
		ULeaderboardCard* LeaderboardCard = CreateWidget<ULeaderboardCard>(GetWorld(), LeaderboardCardClass);
		if (IsValid(LeaderboardCard))
		{
			LeaderboardCard->SetPlayerInfo(LeaderboardItem.username, FString::FromInt(LeaderboardItem.matchWins), FString::FromInt(LeaderboardItem.rank), LeaderboardItem.databaseid);
			ScrollBox_Leaderboard->AddChild(LeaderboardCard);
		}
	}
	
}

void ULeaderboard::SetStatusMessage(const FString& StatusMessage, bool bIsError)
{
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	TextBlock_StatusMessage->SetColorAndOpacity(bIsError ? ErrorColor : SuccessColor);
}

