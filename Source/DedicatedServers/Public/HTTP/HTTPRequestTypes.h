﻿// // Copyright invi1998

#pragma once

#include "HTTPRequestTypes.generated.h"

namespace HTTPStatusMessages
{
	// 定义HTTP状态消息，DEDICATEDSERVERS_API是一个宏，用于处理模块的导出和类和函数的动态链接导入
	extern DEDICATEDSERVERS_API const FString SearchingForGameSession;	// 搜索游戏会话
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong;		// 出现错误
	extern DEDICATEDSERVERS_API const FString FindGameSessionSuccess;	// 找到游戏会话成功
	extern DEDICATEDSERVERS_API const FString GameSessionActivating;	// 游戏会话激活中
	extern DEDICATEDSERVERS_API const FString PlayerSessionCreateFailed;	// 玩家会话创建失败
	extern DEDICATEDSERVERS_API const FString PlayerSessionCreateSuccess;	// 玩家会话创建成功

	// 部分UI文本
	extern DEDICATEDSERVERS_API const FString SignUp;	// 注册
	extern DEDICATEDSERVERS_API const FString ConfirmAccount;	// 确认账户
	extern DEDICATEDSERVERS_API const FString SignIn;	// 登录
	extern DEDICATEDSERVERS_API const FString ResendCode;	// 重新发送验证码
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
		"CreationTime": : "string",
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
		"TerminationTime": "string"
	 }
}
*/

USTRUCT()
struct FDSGameSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};						// 创建时间

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
	FString TerminationTime{};					// 终止时间

	void Dump() const;
};

/*
{
   "PlayerSession": { 
	  "CreationTime": "string",
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
	  "TerminationTime": "string"
   }
}
*/
USTRUCT()
struct FDSPlayerSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};		// 创建时间

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
	FString TerminationTime{};	// 终止时间

	void Dump() const;
};

/*
*
* 注册HTTP回文
{
   "CodeDeliveryDetails": { 
	  "AttributeName": "string",
	  "DeliveryMedium": "string",
	  "Destination": "string"
   },
   "Session": "string",
   "UserConfirmed": boolean,
   "UserSub": "string"
}
 */
USTRUCT()
struct FDSCodeDeliveryDetails
{
	GENERATED_BODY()

	UPROPERTY()
	FString AttributeName{};	// 属性名称

	UPROPERTY()
	FString DeliveryMedium{};	// 交付介质

	UPROPERTY()
	FString Destination{};		// 目的地

	void Dump() const;
};

USTRUCT()
struct FDSSignUpResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FDSCodeDeliveryDetails CodeDeliveryDetails;	// 代码交付细节

	UPROPERTY()
	FString Session{};			// 会话

	UPROPERTY()
	bool UserConfirmed{};		// 用户确认

	UPROPERTY()
	FString UserSub{};			// 用户子
	
	void Dump() const;
};

/*
 * 确认账户请求回文
{
   "Session": "string"
}
 */
USTRUCT()
struct FDSConfirmAccountResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString Session{};	// 会话

	void Dump() const;
};

/*
 * 登录请求回文
{
   "AuthenticationResult": { 
	  "AccessToken": "string",
	  "ExpiresIn": number,
	  "IdToken": "string",
	  "NewDeviceMetadata": { 
		 "DeviceGroupKey": "string",
		 "DeviceKey": "string"
	  },
	  "RefreshToken": "string",
	  "TokenType": "string"
   },
   "AvailableChallenges": [ "string" ],
   "ChallengeName": "string",
   "ChallengeParameters": { 
	  "string" : "string" 
   },
   "Session": "string"
}
 */
USTRUCT()
struct FDSNewDeviceMetadata
{
	GENERATED_BODY()

	UPROPERTY()
	FString DeviceGroupKey{};	// 设备组键

	UPROPERTY()
	FString DeviceKey{};		// 设备键

	void Dump() const;
};

USTRUCT()
struct FDSAuthenticationResult
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccessToken{};	// 访问令牌

	UPROPERTY()
	int32 ExpiresIn{};		// 过期时间

	UPROPERTY()
	FString IdToken{};		// ID令牌

	UPROPERTY()
	FString RefreshToken{};	// 刷新令牌

	UPROPERTY()
	FString TokenType{};		// 令牌类型

	UPROPERTY()
	FDSNewDeviceMetadata NewDeviceMetadata;	// 新设备元数据

	void Dump() const;
};

USTRUCT()
struct FDSUser
{
	GENERATED_BODY()

	UPROPERTY()
	FString Username{};	// 用户名

	UPROPERTY()
	FString Email{};	// 电子邮件

	void Dump() const;
};

USTRUCT()
struct FDSInitiateAuthResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;	// 认证结果

	UPROPERTY()
	TArray<FString> AvailableChallenges;			// 可用挑战

	UPROPERTY()
	FString ChallengeName{};						// 挑战名称

	UPROPERTY()
	TMap<FString, FString> ChallengeParameters;	// 挑战参数

	UPROPERTY()
	FString Session{};							// 会话

	UPROPERTY()
	FDSUser User;								// 用户

	void Dump() const;
};

USTRUCT()
struct FDSMatchStats
{
	GENERATED_BODY()

	UPROPERTY()
	int32 scoredElims{};	// 得分

	UPROPERTY()
	int32 defeats{};		// 失败

	UPROPERTY()
	int32 hits{};			// 击中

	UPROPERTY()
	int32 misses{};			// 错过

	UPROPERTY()
	int32 headShotElims{};	// 爆头击杀

	UPROPERTY()
	int32 highestStreak{};	// 最高连杀

	UPROPERTY()
	int32 revengeElims{};	// 复仇击杀

	UPROPERTY()
	int32 dethroneElims{};	// 排位击杀

	UPROPERTY()
	int32 showStopperElims{};	// 终结者击杀

	UPROPERTY()
	int32 gotFirstBlood{};	// 得到第一滴血

	UPROPERTY()
	int32 matchWins{};		// 比赛胜利

	UPROPERTY()
	int32 matchLosses{};	// 比赛失败
	
};

USTRUCT()
struct FDSRecordMatchStatsInput
{
	GENERATED_BODY()

	UPROPERTY()
	FString userName{};		// 玩家名

	UPROPERTY()
	FDSMatchStats matchStats;	// 比赛统计
};

USTRUCT()
struct FDSRetrieveMatchStatsResponse
{
	GENERATED_BODY()
	UPROPERTY()
	int32 highestStreak{};
	
	UPROPERTY()
	int32 dethroneElims{};
	
	UPROPERTY()
	int32 gotFirstBlood{};
	
	UPROPERTY()
	int32 defeats{};
	
	UPROPERTY()
	int32 scoredElims{};
	
	UPROPERTY()
	FString email{};
	
	UPROPERTY()
	int32 misses{};
	
	UPROPERTY()
	int32 revengeElims{};
	
	UPROPERTY()
	int32 matchLosses{};
	
	UPROPERTY()
	int32 showstopperElims{};
	
	UPROPERTY()
	FString databaseid{};
	
	UPROPERTY()
	int32 headShotElims{};
	
	UPROPERTY()
	FString username{};
	
	UPROPERTY()
	int32 hits{};
	
	UPROPERTY()
	int32 matchWins{};
	
	void Dump() const;
};

/*
{
	"leaderboard": [
	  {
		"databaseid": "691e3498-7071-701a-d8cc-cd494b23be32",
		"username": "test002",
		"matchWins": 2
	  },
	  {
		"databaseid": "998e94d8-e041-7049-35f7-4b42fe748e13",
		"username": "test001",
		"matchWins": 1
	  }
	]
  }
*/
USTRUCT()
struct FDSLeaderboardItem
{
	GENERATED_BODY()

	UPROPERTY()
	FString databaseid{};	// 用户SubID

	UPROPERTY()
	FString username{};		// 用户名

	UPROPERTY()
	int32 matchWins{};		// 比赛胜利场数

	UPROPERTY()
	int32 rank{};			// 排名

	void Dump() const;
};



