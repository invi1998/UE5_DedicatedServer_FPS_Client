// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "PortalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBroadcastJoinGameSessionMessage, const FString&, StatusMessage, bool, bNeedRestJoinGameButton);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBroadcastJoinGameSessionMessage OnJoinGameSessionMessage;
	
	void JoinGameSession(bool bRetry = false);


private:
	/** 
	 * @brief 处理游戏会话响应
	 * @param Request 请求
	 * @param Response 响应
	 * @param bWasSuccessful 是否成功
	 */
	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/** 
	 * @brief 创建玩家会话响应
	 * @param Request 请求
	 * @param Response 响应
	 * @param bWasSuccessful 是否成功
	 */
	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	FString GetUniquePlayerID() const;
	void HandleGameSessionStatus(const FString& Status, const FString& SessionId);
	void TryCreatePlayerSession(const FString& GameSessionId, const FString& PlayerId);

	FTimerHandle CreateGameSessionTimerHandle;		// 创建游戏会话定时器句柄

	
};
