﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Types/DSTypes.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerStateChangeDelegate, float, Time, ECountdownTimerType, TimerType);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADSPlayerController();

	virtual void ReceivedPlayer() override;	// 玩家控制器初始化
	virtual void OnRep_PlayerState() override;	// 玩家状态复制
	virtual void PostSeamlessTravel() override;	// 无缝旅行结束

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void Client_TimerUpdate(float CountdownTimeLeft, ECountdownTimerType TimerType);	// 客户端定时器更新

	UFUNCTION(Client, Reliable)
	void Client_TimerFinished(float CountdownTimeLeft, ECountdownTimerType TimerType);	// 客户端定时器结束

	UFUNCTION(Client, Reliable)
	void Client_TimerPaused(float CountdownTimeLeft, ECountdownTimerType TimerType);	// 客户端定时器暂停

	UFUNCTION(Client, Reliable)
	void Client_TimerResumed(float CountdownTimeLeft, ECountdownTimerType TimerType);	// 客户端定时器恢复

	UFUNCTION(Client, Reliable)
	void Client_SetInputEnabled(bool bEnable);	// 设置输入是否可用

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangeDelegate OnTimerUpdated;	// 定时器更新委托

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangeDelegate OnTimerFinished;	// 定时器结束委托

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangeDelegate OnTimerPaused;	// 定时器暂停委托

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangeDelegate OnTimerResumed;	// 定时器恢复委托

	UPROPERTY(BlueprintReadOnly)
	FString PlayerSessionId{};	// 玩家会话ID

	UPROPERTY(BlueprintReadOnly)
	FString UserName{};	// 用户名

protected:

	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeStamp);

	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeStamp);
	
private:
	float SingleTripTime = 0.0f;	// 单程时间
};
