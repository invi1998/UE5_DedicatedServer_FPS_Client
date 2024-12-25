// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_MatchGameMode.generated.h"

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

	UPROPERTY()
	EMatchStatus MatchStatus;	// 比赛状态

protected:
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;	// 比赛前定时器句柄
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;	// 比赛定时器句柄

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;	// 比赛后定时器句柄

	virtual void OnCountdownTimerFinished(ECountdownTimerType InTimerType) override;
	
private:
	
};
