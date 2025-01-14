// // Copyright invi1998


#include "UI/portal/Dashboard/LeaderboardCard.h"

#include "Components/TextBlock.h"

void ULeaderboardCard::SetPlayerInfo(const FString& UserName, const FString& WinCount, const FString& Rank, const FString& SubId) const
{
	TextBlock_UserName->SetText(FText::FromString(UserName));
	TextBlock_WinCount->SetText(FText::FromString(WinCount));
	TextBlock_Rank->SetText(FText::FromString(Rank));
	TextBlock_SubId->SetText(FText::FromString(SubId));
}
