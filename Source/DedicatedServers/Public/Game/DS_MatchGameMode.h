// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"

class UGameStatsManager;
/**
 * 主要用于处理比赛的游戏模式
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:
	ADS_MatchGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	UPROPERTY()
	EMatchStatus MatchStatus;	// 比赛状态

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

protected:
	virtual void BeginPlay() override;
	
	virtual void OnCountdownTimerFinished(ECountdownTimerType InTimerType) override;
	virtual void OnMatchTimerFinished();
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;	// 比赛前定时器句柄
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;	// 比赛定时器句柄

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;	// 比赛后定时器句柄

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;	// 无缝旅行目标地图(大厅地图)
	
	void SetClientInputEnabled(bool bEnable);
	void FinishMatchForPlayerStates() const;
	void UpdateLeaderboard(const TArray<FString>& WinnerPlayerNames);

	UFUNCTION()
	void OnLeaderboardUpdated();
	
private:
	
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
};

