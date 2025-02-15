﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "HTTP/HTTPRequestManager.h"
#include "HTTP/HTTPRequestTypes.h"
#include "Interfaces/PortalManagement.h"
#include "PortalManager.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager, public IPortalManagement
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage OnSignUpStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage OnSignInStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage OnConfirmAccountStatusMessageDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FAPIRequestComplete OnSignUpCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIRequestComplete OnSignInCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIRequestComplete OnConfirmAccountCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIRequestComplete OnSuccessConfirmCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIRequestComplete OnSignOutCompleteDelegate;
	
	UFUNCTION()
	void QuitGame();
	
	void SignIn(const FString& Username, const FString& Password);
	void SignUp(const FString& Username, const FString& Email, const FString& FullName, const FString& Password);
	void ConfirmAccount(const FString& Code, const FString& Username);
	void ResendCode();
	void SignOut(const FString& AccessToken);

	FDSSignUpResponse LastSignUpResponse{};
	FString LastSignUpUsername{};
	FString LastSignUpPassword{};

	// IPortalManagement interface
	// 刷新令牌
	virtual void RefreshToken(const FString &RefreshToken) override;

private:
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void ConfirmAccount_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void ResendCode_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FTimerHandle SwitchToConfirmAccountPageTimerHandle;
	
};
