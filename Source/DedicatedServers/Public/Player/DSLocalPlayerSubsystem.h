// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubsystem.generated.h"

class UPortalManager;
/**
 * 本地玩家子系统，用于实现玩家数据的存储和管理
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void InitializeToken(const FDSAuthenticationResult& InAuthenticationResult, UPortalManager* InPortalManager);
	const FDSAuthenticationResult& GetAuthenticationResult() const { return AuthenticationResult; }

private:
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;

	FTimerHandle RefrushTokenTimerHandle;

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;
	
};
