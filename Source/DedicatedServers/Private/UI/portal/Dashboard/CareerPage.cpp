// // Copyright invi1998


#include "UI/portal/Dashboard/CareerPage.h"

#include "Components/TextBlock.h"

void UCareerPage::OnRetrieveMatchStatsReceived(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse)
{
}

void UCareerPage::SetMatchStats(const FString& StatusStr, bool bNeedResetButton)
{
	TextBlock_Status->SetText(FText::FromString(StatusStr));
}

void UCareerPage::NativeConstruct()
{
	Super::NativeConstruct();
}
