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
	
	void JoinGameSession();


private:
	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
