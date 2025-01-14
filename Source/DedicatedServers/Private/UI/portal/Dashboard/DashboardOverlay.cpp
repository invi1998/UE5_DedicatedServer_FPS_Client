// // Copyright invi1998


#include "UI/portal/Dashboard/DashboardOverlay.h"

#include "CommonButtonBase.h"
#include "Components/WidgetSwitcher.h"
#include "UI/GameStats/GameStatsManager.h"
#include "UI/portal/Dashboard/CareerPage.h"
#include "UI/portal/Dashboard/GamePage.h"
#include "UI/portal/Dashboard/Leaderboard.h"
#include "UI/portal/Dashboard/SettingPage.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	// 绑定按钮点击事件
	// 因为Button_Game是UE5的CommonUI按钮
	Button_Game->OnClicked().AddUObject(this, &UDashboardOverlay::ShowGamePage);
	Button_Career->OnClicked().AddUObject(this, &UDashboardOverlay::ShowCareerPage);
	Button_Leaderboard->OnClicked().AddUObject(this, &UDashboardOverlay::ShowLeaderboard);
	Button_Setting->OnClicked().AddUObject(this, &UDashboardOverlay::ShowSettingPage);

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	if (IsValid(GameStatsManager))
	{
		GameStatsManager->OnRetrieveMatchStatsReceived.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStatsReceived);
		GameStatsManager->RetrieveMatchStatsStatusMessageDelegate.AddDynamic(CareerPage, &UCareerPage::SetMatchStats);
		GameStatsManager->OnRetrieveLeaderboardReceived.AddDynamic(Leaderboard, &ULeaderboard::PopulateLeaderboard);
		GameStatsManager->RetrieveLeaderboardStatusMessageDelegate.AddDynamic(Leaderboard, &ULeaderboard::SetStatusMessage);
	}

	ShowGamePage();
}

void UDashboardOverlay::ShowGamePage()
{
	// DisableButton(Button_Game);
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	// DisableButton(Button_Career);
	
	WidgetSwitcher->SetActiveWidget(CareerPage);
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::ShowLeaderboard()
{
	// DisableButton(Button_Leaderboard);
	WidgetSwitcher->SetActiveWidget(Leaderboard);
	GameStatsManager->RetrieveLeaderboard();
}

void UDashboardOverlay::ShowSettingPage()
{
	// DisableButton(Button_Setting);
	WidgetSwitcher->SetActiveWidget(SettingPage);
}

void UDashboardOverlay::DisableButton(UCommonButtonBase* Button)
{
	Button_Career->SetIsEnabled(true);
	Button_Game->SetIsEnabled(true);
	Button_Leaderboard->SetIsEnabled(true);
	Button_Setting->SetIsEnabled(true);

	Button->SetIsEnabled(false);
}

