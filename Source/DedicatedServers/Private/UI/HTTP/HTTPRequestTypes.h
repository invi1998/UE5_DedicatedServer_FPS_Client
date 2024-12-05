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

USTRUCT()
struct FDSListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds;	// 舰队ID数组

	UPROPERTY()
	FString NextToken;			// 下一个令牌

	void Dump() const;
};

/*
{
	"GameSession": { 
		"CreationTime": number,
		"CreatorId": "string",
		"CurrentPlayerSessionCount": number,
		"DnsName": "string",
		"FleetArn": "string",
		"FleetId": "string",
		"GameProperties": [ 
		   { 
			  "Key": "string",
			  "Value": "string"
		   }
		],
		"GameSessionData": "string",
		"GameSessionId": "string",
		"IpAddress": "string",
		"Location": "string",
		"MatchmakerData": "string",
		"MaximumPlayerSessionCount": number,
		"Name": "string",
		"PlayerSessionCreationPolicy": "string",
		"Port": number,
		"Status": "string",
		"StatusReason": "string",
		"TerminationTime": number
	 }
}
*/

USTRUCT()
struct FDSGameSession
{
	GENERATED_BODY()

	UPROPERTY()
	int64 CreationTime{};						// 创建时间

	UPROPERTY()
	FString CreatorId{};						// 创建者ID

	UPROPERTY()
	int32 CurrentPlayerSessionCount{};			// 当前玩家会话计数

	UPROPERTY()
	FString DnsName{};							// DNS名称

	UPROPERTY()
	FString FleetArn{};							// 舰队ARN

	UPROPERTY()
	FString FleetId{};							// 舰队ID

	UPROPERTY()
	TMap<FString, FString> GameProperties{};	// 游戏属性

	UPROPERTY()
	FString GameSessionData{};					// 游戏会话数据

	UPROPERTY()
	FString GameSessionId{};					// 游戏会话ID

	UPROPERTY()
	FString IpAddress{};						// IP地址

	UPROPERTY()
	FString Location{};							// 位置

	UPROPERTY()
	FString MatchmakerData{};					// 匹配数据

	UPROPERTY()
	int32 MaximumPlayerSessionCount{};			// 最大玩家会话计数

	UPROPERTY()
	FString Name{};								// 名称

	UPROPERTY()
	FString PlayerSessionCreationPolicy{};		// 玩家会话创建策略

	UPROPERTY()
	int32 Port{};								// 端口

	UPROPERTY()
	FString Status{};							// 状态

	UPROPERTY()
	FString StatusReason{};						// 状态原因

	UPROPERTY()
	int64 TerminationTime{};					// 终止时间

	void Dump() const;
};

