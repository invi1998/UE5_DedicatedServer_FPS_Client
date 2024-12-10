// // Copyright invi1998


#include "UI/portal/Dashboard/DashboardOverlay.h"

#include "CommonButtonBase.h"
#include "Components/WidgetSwitcher.h"
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
}

void UDashboardOverlay::ShowGamePage()
{
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	WidgetSwitcher->SetActiveWidget(CareerPage);
}

void UDashboardOverlay::ShowLeaderboard()
{
	WidgetSwitcher->SetActiveWidget(Leaderboard);
}

void UDashboardOverlay::ShowSettingPage()
{
	WidgetSwitcher->SetActiveWidget(SettingPage);
}
