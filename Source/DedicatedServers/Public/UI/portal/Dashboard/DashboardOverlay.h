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
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;		// 小部件切换器

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGamePage> GamePage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCareerPage> CareerPage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<ULeaderboard> Leaderboard;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USettingPage> SettingPage;

	// 按钮
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UUserWidget> GameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UUserWidget> CareerButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UUserWidget> LeaderboardButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UUserWidget> SettingButton;
};
