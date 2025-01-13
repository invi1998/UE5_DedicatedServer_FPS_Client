// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "HttpFwd.h"
#include "HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"

struct FDSLeaderboardItem;
struct FDSRetrieveMatchStatsResponse;
struct FDSRecordMatchStatsInput;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveMatchStatsReceived, const FDSRetrieveMatchStatsResponse&, RetrieveMatchStatsResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveLeaderboardReceived, const TArray<FDSLeaderboardItem>&, RetrieveLeaderboardResponse);


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

	void UpdateLeaderboard(const TArray<FString> &WinnerPlayerNames);
	void RetrieveLeaderboard();

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage RetrieveMatchStatsStatusMessageDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnRetrieveMatchStatsReceived OnRetrieveMatchStatsReceived;	// 当接收到比赛统计数据时触发

	UPROPERTY(BlueprintAssignable)
	FAPIRequestComplete OnUpdateLeaderboardComplete;

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage RetrieveLeaderboardStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnRetrieveLeaderboardReceived OnRetrieveLeaderboardReceived;	// 当接收到排行榜数据时触发

private:
	void RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void UpdateLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrieveLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
