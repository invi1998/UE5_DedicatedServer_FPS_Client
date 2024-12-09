// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "HTTP/HTTPRequestManager.h"
#include "APITestManger.generated.h"

// 舰队列表回文响应委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFleetsResponseReceived, const FDSListFleetsResponse&, ListFleetsResponse, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestManger : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ListFleets();

	UPROPERTY()
	FOnListFleetsResponseReceived OnListFleetsResponseReceived;

	// HTTP请求回调
	void ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
private:
	
};
