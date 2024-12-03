// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "APIData.generated.h"

/**
 * api 配置数据
 */
UCLASS()
class DEDICATEDSERVERS_API UAPIData : public UDataAsset
{
	GENERATED_BODY()

public:
	FString GetAPIEndPoint(const FGameplayTag& Resource) const;

protected:
	// 我们的完整url：InvokeURL/Stage/Resource

	// API名称（用于显示）
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	// API调用URL
	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;

	// API调用阶段
	UPROPERTY(EditDefaultsOnly)
	FString Stage;

	// 该Map用于存储API资源（key：API标签名，value：API最后的字段名）
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> Resources;
	
};
