// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DSTypes.h"
#include "DS_GameModeBase.generated.h"

/**
 * 该类用于提供定时器功能
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
	void StartCountdownTimer(FCountdownTimerHandle& TimerHandle);		// 开始定时器
	void PauseCountdownTimer(FCountdownTimerHandle& TimerHandle);		// 暂停定时器
	void ResumeCountdownTimer(FCountdownTimerHandle& TimerHandle);		// 恢复定时器
	void StopCountdownTimer(FCountdownTimerHandle& TimerHandle);			// 停止定时器

	void UpdateCountdownTimer(const FCountdownTimerHandle& TimerHandle);		// 更新定时器

	virtual void OnCountdownTimerFinished(ECountdownTimerType InTimerType);	// 定时器结束
	
};

