// // Copyright invi1998

#include "HTTPRequestTypes.h"

#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessages
{
	const FString SearchingForGameSession = TEXT("游戏会话搜索中（Searching for game session）...");
	const FString SomethingWentWrong = TEXT("出现错误（Something went wrong）");
	const FString FindGameSessionSuccess = TEXT("成功找到游戏会话，正在创建玩家会话（Find game session successful, creating player session）...");
	const FString GameSessionActivating = TEXT("游戏会话激活中（Game session activating）...");
	const FString PlayerSessionCreateFailed = TEXT("玩家会话创建失败（Player session create failed）");
	const FString PlayerSessionCreateSuccess = TEXT("玩家会话创建成功，正在进入游戏（Player session create successful, entering game）...");

	const FString SignUp = TEXT("注册（Sign up）");
	const FString ConfirmAccount = TEXT("确认（Confirm account）");
	const FString SignIn = TEXT("登录（Sign in）");
	const FString ResendCode = TEXT("重新发送验证码（Resend code）");
}

void FDSMeataData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("metadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	HTTP Status Code: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Request ID: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Total Retry Delay: %f"), totalRetryDelay);
}

void FDSListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ListFleetsResponse:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	FleetIds:"));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("	  %s"), *FleetId);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("	NextToken: %s"), *NextToken);
}

void FDSGameSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("	CreatorId: %s"), *CreatorId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("	DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("	FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("	FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	GameProperties:"));
	for (const TPair<FString, FString>& GameProperty : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("	  %s: %s"), *GameProperty.Key, *GameProperty.Value);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("	GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("	GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("	MatchmakerData: %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("	MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("	PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("	StatusReason: %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("	TerminationTime: %s"), *TerminationTime);
	
}

void FDSPlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("	DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("	FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("	FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("	PlayerData: %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("	PlayerId: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("	TerminationTime: %s"), *TerminationTime);
	
}

void FDSCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("CodeDeliveryDetails:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	AttributeName: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("	DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("	Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("SignUpResponse:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	CodeDeliveryDetails:"));
	CodeDeliveryDetails.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("	Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("	UserConfirmed: %d"), UserConfirmed);
	UE_LOG(LogDedicatedServers, Log, TEXT("	UserSub: %s"), *UserSub);
}

void FDSConfirmAccountResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ConfirmAccountResponse:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	Session: %s"), *Session);
}

void FDSNewDeviceMetadata::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("NewDeviceMetadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("	DeviceKey: %s"), *DeviceKey);
	
}

void FDSAuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("AuthenticationResult:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	AccessToken: %s"), *AccessToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("	ExpiresIn: %d"), ExpiresIn);
	UE_LOG(LogDedicatedServers, Log, TEXT("	IdToken: %s"), *IdToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("	RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("	TokenType: %s"), *TokenType);
	UE_LOG(LogDedicatedServers, Log, TEXT("	NewDeviceMetadata:"));
	NewDeviceMetadata.Dump();
}

void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("InitiateAuthResponse(SignIn):"));
	UE_LOG(LogDedicatedServers, Log, TEXT("	AuthenticationResult:"));
	AuthenticationResult.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("	AvailableChallenges:"));
	for (const FString& AvailableChallenge : AvailableChallenges)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("	  %s"), *AvailableChallenge);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("	ChallengeName: %s"), *ChallengeName);
	for (const TPair<FString, FString>& Parameter : ChallengeParameters)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("	  %s: %s"), *Parameter.Key, *Parameter.Value);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("	Session: %s"), *Session);
}
