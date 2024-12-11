// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubsystem.generated.h"

class IPortalManagement;
/**
 * 本地玩家子系统，用于实现玩家数据的存储和管理
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void InitializeToken(const FDSAuthenticationResult& InAuthenticationResult, TScriptInterface<IPortalManagement> InPortalManager);
	
	void SetRefreshTokenTimer();

	void UpdateToken(const FString& AccessToken, const FString& IdToken);

	FDSAuthenticationResult GetAuthenticationResult() const;

	FString UserName{};
	FString Email{};
	
private:
	void RefreshToken();
	
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;
	
	UPROPERTY()
	TScriptInterface<IPortalManagement> PortalManagementInterface;

	FTimerHandle RefreshTokenTimerHandle;
	
	float TokenRefreshInterval = 2700.0f;		// 令牌刷新间隔，单位秒，45分钟（AWS Cognito令牌有效期为1小时）
	
};
