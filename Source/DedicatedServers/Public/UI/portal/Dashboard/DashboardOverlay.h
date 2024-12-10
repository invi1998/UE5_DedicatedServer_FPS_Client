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
class UCommonButtonBase;
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

	// UE5通用按钮基类：CommonButtonBase
	
	// BlueprintProtected = true 表示这个变量只能在蓝图中访问
	// AllowPrivateAccess = true 表示这个变量只能在这个类中访问
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_Game;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_Career;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_Leaderboard;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_Setting;

	UFUNCTION()
	void ShowGamePage();

	UFUNCTION()
	void ShowCareerPage();

	UFUNCTION()
	void ShowLeaderboard();

	UFUNCTION()
	void ShowSettingPage();

protected:
	 virtual void NativeConstruct() override;

private:

};
