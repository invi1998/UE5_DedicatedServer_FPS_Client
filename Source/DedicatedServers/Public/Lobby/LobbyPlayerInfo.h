// // Copyright invi1998

#pragma once

#include "Net/Serialization/FastArraySerializer.h"
#include "LobbyPlayerInfo.generated.h"

// 使用 FastArraySerializer 来序列化 TArray<FLobbyPlayerInfo>，这样可以在客户端和服务器之间高效地传输数据变化
// 因为 TArray 不支持差异化序列化，所以我们需要使用 FastArraySerializer 来实现
// FastArraySerializer 可以在客户端和服务器之间高效地传输数据变化，而不是每次都发送整个数据集。这样可以显著减少网络流量，并提高性能

USTRUCT(BlueprintType)
struct FLobbyPlayerInfo : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FLobbyPlayerInfo() = default;
	FLobbyPlayerInfo(const FString& InPlayerSessionId, const FString& InUserName)
		: PlayerSessionId(InPlayerSessionId)
		, UserName(InUserName)
	{
	}

	UPROPERTY(BlueprintReadWrite)
	FString PlayerSessionId{};

	UPROPERTY(BlueprintReadWrite)
	FString UserName{};
	
};

USTRUCT()
struct FLobbyPlayerInfoArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLobbyPlayerInfo> Players;

	// 它的主要目的是在客户端和服务器之间高效地传输数据变化，而不是每次都发送整个数据集。这样可以显著减少网络流量，并提高性能
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		// 处理 TArray 的差异化序列化
		return FFastArraySerializer::FastArrayDeltaSerialize<FLobbyPlayerInfo, FLobbyPlayerInfoArray>(Players, DeltaParms, *this);
	}

	// 添加自定义的功能函数
	void AddPlayer(const FLobbyPlayerInfo& Player);
	void RemovePlayer(const FString& PlayerSessionId);
	
};

// 为了使 TArray<FLobbyPlayerInfo> 支持差异化序列化，我们需要为其定义 TStructOpsTypeTraits
// 启用 NetDeltaSerializer 特性
template<>
struct TStructOpsTypeTraits<FLobbyPlayerInfoArray> : public TStructOpsTypeTraitsBase2<FLobbyPlayerInfoArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

