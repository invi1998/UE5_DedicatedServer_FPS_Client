// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "HttpFwd.h"
#include "HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"

struct FDSRetrieveMatchStatsResponse;
struct FDSRecordMatchStatsInput;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveMatchStatsReceived, const FDSRetrieveMatchStatsResponse&, RetrieveMatchStatsResponse);


/**
 * 统计游戏比赛数据管理器
 */
UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	void RecordMatchStats(const FDSRecordMatchStatsInput& MatchStatsInput);
	void RetrieveMatchStats();

	UPROPERTY(BlueprintAssignable)
	FOnRetrieveMatchStatsReceived OnRetrieveMatchStatsReceived;	// 当接收到比赛统计数据时触发

private:
	void RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
