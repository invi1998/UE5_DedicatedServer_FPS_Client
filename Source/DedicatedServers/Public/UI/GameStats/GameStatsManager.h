// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"

struct FDSRecordMatchStatsInput;

/**
 * 统计游戏比赛数据管理器
 */
UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	void RecordMatchStats(const FDSRecordMatchStatsInput& MatchStatsInput);
};
