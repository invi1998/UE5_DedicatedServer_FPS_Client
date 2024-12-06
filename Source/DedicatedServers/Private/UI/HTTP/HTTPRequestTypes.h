// // Copyright invi1998

#pragma once

#include "HTTPRequestTypes.generated.h"

namespace HTTPStatusMessages
{
	// 定义HTTP状态消息，DEDICATEDSERVERS_API是一个宏，用于处理模块的导出和类和函数的动态链接导入
	extern DEDICATEDSERVERS_API const FString SearchingForGameSession;	// 搜索游戏会话
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong;		// 出现错误
	extern DEDICATEDSERVERS_API const FString FindGameSessionSuccess;	// 找到游戏会话成功
	extern DEDICATEDSERVERS_API const FString GameSessionActivating;	// 游戏会话激活中
	
}

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

/*
{
   "PlayerSession": { 
	  "CreationTime": number,
	  "DnsName": "string",
	  "FleetArn": "string",
	  "FleetId": "string",
	  "GameSessionId": "string",
	  "IpAddress": "string",
	  "PlayerData": "string",
	  "PlayerId": "string",
	  "PlayerSessionId": "string",
	  "Port": number,
	  "Status": "string",
	  "TerminationTime": number
   }
}
*/
USTRUCT()
struct FDSPlayerSession
{
	GENERATED_BODY()

	UPROPERTY()
	int64 CreationTime{};		// 创建时间

	UPROPERTY()
	FString DnsName{};			// DNS名称

	UPROPERTY()
	FString FleetArn{};			// 舰队ARN

	UPROPERTY()
	FString FleetId{};			// 舰队ID

	UPROPERTY()
	FString GameSessionId{};		// 游戏会话ID

	UPROPERTY()
	FString IpAddress{};		// IP地址

	UPROPERTY()
	FString PlayerData{};		// 玩家数据

	UPROPERTY()
	FString PlayerId{};			// 玩家ID

	UPROPERTY()
	FString PlayerSessionId{};	// 玩家会话ID

	UPROPERTY()
	int32 Port{};				// 端口

	UPROPERTY()
	FString Status{};			// 状态

	UPROPERTY()
	int64 TerminationTime{};	// 终止时间

	void Dump() const;
};

