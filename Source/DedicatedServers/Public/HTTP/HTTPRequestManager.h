﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTPRequestManager.generated.h"

class UDSLocalPlayerSubsystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAPIStatusMessage, const FString&, StatusMessage, bool, bNeedRestButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIRequestComplete);


class UAPIData;
/**
 * HTTP 请求管理器
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()

public:
	UDSLocalPlayerSubsystem* GetDSLocalPlayerSubsystem() const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> APIData;

	static bool ContainErrors(TSharedPtr<FJsonObject> JsonObject);

	static void DumpMetaData(TSharedPtr<FJsonObject> JsonObject);

	static FString SerializeJsonContent(const TMap<FString, FString>& Params);
	
};
