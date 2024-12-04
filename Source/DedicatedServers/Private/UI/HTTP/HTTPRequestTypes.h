// // Copyright invi1998

#pragma once

#include "HTTPRequestTypes.generated.h"

USTRUCT()
struct FDSMeataData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 httpStatusCode{};		// HTTP状态码

	UPROPERTY()
	FString requestId{};		// 请求ID

	UPROPERTY()
	int32 attempts{};			// 尝试次数

	UPROPERTY()
	double totalRetryDelay{};	// 总重试延迟

	void Dump() const;
};
