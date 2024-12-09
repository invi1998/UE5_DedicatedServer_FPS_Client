// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubsystem.generated.h"

/**
 * 本地玩家子系统，用于实现玩家数据的存储和管理
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

private:
	FDSAuthenticationResult AuthenticationResult;
};
