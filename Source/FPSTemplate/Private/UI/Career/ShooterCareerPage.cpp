// // Copyright invi1998


#include "UI/Career/ShooterCareerPage.h"

#include "Components/ScrollBox.h"
#include "Data/SpecialElimData.h"
#include "HTTP/HTTPRequestTypes.h"
#include "ShooterTypes/ShooterTypes.h"
#include "UI/portal/Dashboard/DSImageButtun.h"

void UShooterCareerPage::OnRetrieveMatchStatsReceived(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse)
{
	Super::OnRetrieveMatchStatsReceived(RetrieveMatchStatsResponse);

	ScrollBox_Achievements->ClearChildren();

	TMap<ESpecialElimType, int32> AchievementsData;
	
	AchievementsData.Emplace(ESpecialElimType::Hits, RetrieveMatchStatsResponse.hits);
	AchievementsData.Emplace(ESpecialElimType::Misses, RetrieveMatchStatsResponse.misses);
	AchievementsData.Emplace(ESpecialElimType::Defeats, RetrieveMatchStatsResponse.defeats);
	AchievementsData.Emplace(ESpecialElimType::ScoredElims, RetrieveMatchStatsResponse.scoredElims);
	
	AchievementsData.Emplace(ESpecialElimType::Streak, RetrieveMatchStatsResponse.highestStreak);
	AchievementsData.Emplace(ESpecialElimType::Dethrone, RetrieveMatchStatsResponse.dethroneElims);
	AchievementsData.Emplace(ESpecialElimType::FirstBlood, RetrieveMatchStatsResponse.gotFirstBlood);
	AchievementsData.Emplace(ESpecialElimType::Revenge, RetrieveMatchStatsResponse.revengeElims);
	AchievementsData.Emplace(ESpecialElimType::Showstopper, RetrieveMatchStatsResponse.showstopperElims);
	AchievementsData.Emplace(ESpecialElimType::Headshot, RetrieveMatchStatsResponse.headShotElims);
	
	check(SpecialElimData);

	for (const TPair<ESpecialElimType, int32>& AchievementData : AchievementsData)
	{
		const FString& AchievementName = SpecialElimData->SpecialElimInfo.FindChecked(AchievementData.Key).CareerPageAchievementName;
		const FString& AchievementCNName = SpecialElimData->SpecialElimInfo.FindChecked(AchievementData.Key).CareerPageAchievementCNName;
		UTexture2D* AchievementIcon = SpecialElimData->SpecialElimInfo.FindChecked(AchievementData.Key).ElimIcon;

		UDSImageButtun* CareerAchievementButton = CreateWidget<UDSImageButtun>(GetWorld(), CareerAchievementButtonClass);
		CareerAchievementButton->UpdateTexture(AchievementIcon);
		CareerAchievementButton->SetTitle(AchievementName);
		CareerAchievementButton->SetCNTitle(AchievementCNName);
		CareerAchievementButton->SetDescription(FString::Printf(TEXT("%d"), AchievementData.Value));
		ScrollBox_Achievements->AddChild(CareerAchievementButton);
	}
}

void UShooterCareerPage::NativeConstruct()
{
	Super::NativeConstruct();
	
}
