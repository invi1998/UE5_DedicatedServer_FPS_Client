﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DSTypes.h"
#include "DS_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
	void StartCountdownTimer(FCountdownTimerHandle TimerHandle);		// 开始定时器
	void PauseCountdownTimer(FCountdownTimerHandle TimerHandle);		// 暂停定时器
	void ResumeCountdownTimer(FCountdownTimerHandle TimerHandle);		// 恢复定时器
	void StopCountdownTimer(FCountdownTimerHandle TimerHandle);			// 停止定时器

	virtual void OnCountdownTimerFinished(ECountdownTimerType TimerType);	// 定时器结束
	
};