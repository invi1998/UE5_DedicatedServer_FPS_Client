// // Copyright invi1998


#include "UI/portal/Dashboard/CareerPage.h"

#include "Components/TextBlock.h"
#include "HTTP/HTTPRequestTypes.h"

void UCareerPage::OnRetrieveMatchStatsReceived(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse)
{
	TextBlock_UserName->SetText(FText::FromString(RetrieveMatchStatsResponse.username));
	TextBlock_Wins->SetText(FText::FromString(FString::FromInt(RetrieveMatchStatsResponse.matchWins)));
	TextBlock_Losses->SetText(FText::FromString(FString::FromInt(RetrieveMatchStatsResponse.matchLosses)));
}

void UCareerPage::SetMatchStats(const FString& StatusStr, bool bNeedResetButton)
{
	TextBlock_Status->SetText(FText::FromString(StatusStr));
}

void UCareerPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_UserName->SetText(FText::FromString(TEXT("")));
	TextBlock_Wins->SetText(FText::FromString(TEXT("")));
	TextBlock_Losses->SetText(FText::FromString(TEXT("")));
}
