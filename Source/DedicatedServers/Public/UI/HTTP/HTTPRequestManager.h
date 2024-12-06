// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTPRequestManager.generated.h"

class UAPIData;
/**
 * HTTP 请求管理器
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> APIData;

	static bool ContainErrors(TSharedPtr<FJsonObject> JsonObject);

	static void DumpMetaData(TSharedPtr<FJsonObject> JsonObject);

	static FString SerializeJsonContent(const TMap<FString, FString>& Params);

};
