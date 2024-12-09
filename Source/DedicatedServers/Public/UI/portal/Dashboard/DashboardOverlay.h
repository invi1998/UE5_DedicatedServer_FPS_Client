// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class USettingPage;
class ULeaderboard;
class UCareerPage;
class UGamePage;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGamePage> GamePage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCareerPage> CareerPage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULeaderboard> Leaderboard;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USettingPage> SettingPage;

	// 按钮
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> GameButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CareerButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeaderboardButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SettingButton;
};
