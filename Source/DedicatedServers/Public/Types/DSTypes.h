﻿#pragma once

#include "DSTypes.generated.h"

UENUM(BlueprintType)
enum class ECountdownTimerState : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"),
	Started UMETA(DisplayName = "Started"),
	Paused UMETA(DisplayName = "Paused"),
	Finished UMETA(DisplayName = "Finished")
};

UENUM(BlueprintType)
enum class ECountdownTimerType : uint8
{
	LobbyCountdown UMETA(DisplayName = "Lobby Countdown"),
	PreMatchCountdown UMETA(DisplayName = "Pre-Match Countdown"),
	MatchCountdown UMETA(DisplayName = "Match Countdown"),
	PostMatchCountdown UMETA(DisplayName = "Post-Match Countdown"),
	NONE UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FCountdownTimerHandle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerState State = ECountdownTimerState::NotStarted;	// 定时器当前状态

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerType Type = ECountdownTimerType::NONE;	// 定时器类型

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 0.0f;	// 定时器持续时间
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float UpdateInterval = 1.0f;	// 定时器更新间隔

	FTimerHandle TimerFinishedHandle;	// 定时器结束句柄
	FTimerHandle UpdateTimerHandle;	// 定时器更新句柄

	FTimerDelegate TimerFinishedDelegate;	// 定时器结束委托
	FTimerDelegate UpdateTimerDelegate;	// 定时器更新委托
	
};

// 定时器比较
FORCEINLINE bool operator==(const FCountdownTimerHandle& A, const FCountdownTimerHandle& B)
{
	return A.Type == B.Type;
}
