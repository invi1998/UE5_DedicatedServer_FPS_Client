// // Copyright invi1998


#include "Game/DS_LobbyGameMode.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSPlayerController.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true;
	LobbyStatus = ELobbyStatus::WaitingForPlayers;
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
	LobbyCountdownTimer.Duration = 10.0f;
	LobbyCountdownTimer.UpdateInterval = 1.0f;
	
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	CheckAndStartLobbyCountdown();
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	CheckAndStopLobbyCountdown();
	RemovePlayerSession(Exiting);

}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// 解析玩家登录参数
	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));

	TryAcceptPlayerSession(PlayerSessionId, UserName, ErrorMessage);
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	// 这里我们在初始化玩家时，将玩家会话ID和用户名传递给玩家控制器进行保存
	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));

	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController);
	if (IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->UserName = UserName;
	}
	
	return InitializedString;
	
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InitGameLift();
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	CheckAndStartLobbyCountdown();
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType InTimerType)
{
	Super::OnCountdownTimerFinished(InTimerType);

	if (InTimerType == ECountdownTimerType::LobbyCountdown)
	{
		// 无缝旅行
		if (LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
		{
			LobbyStatus = ELobbyStatus::SeamlessTravel;
			TrySeamlessTravel(DestinationMap);
		}
	}
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountdown()
{
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		// 开始无缝旅行
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::CheckAndStopLobbyCountdown()
{
	if (GetNumPlayers()-1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		// 取消无缝旅行
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>();
		if (IsValid(DSGameInstanceSubsystem))
		{
			// 从命令行参数中解析端口等信息
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		}
	}
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	// GameLift Anywhere 运行在本地的服务器
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	// 进程ID
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), FPlatformProcess::GetCurrentProcessId());
	UE_LOG(LogDedicatedServers, Log, TEXT("PROCESS_ID [PID]: %s"), *OutServerParameters.m_processId)
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& UserName, FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || UserName.IsEmpty())
	{
		OutErrorMessage = TEXT("PlayerSessionId or UserName is empty");
		return;
	}

	// 调用GameLift接口，接受玩家会话
#if WITH_GAMELIFT
	// GameLift SDK
	// 如何接收玩家会话？
	// 1. 调用GameLiftServerSDK::DescribePlayerSessions，获取玩家会话信息
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));	// TCHAR_TO_ANSI 将FString转换为char*
	// 获取玩家会话信息
	const Aws::GameLift::DescribePlayerSessionsOutcome& DescribePlayerSessionOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("Failed to describe player session");
		return;
	}
	// 2. 比较玩家会话ID，如果匹配，则接受玩家会话
	const Aws::GameLift::Server::Model::DescribePlayerSessionsResult& DescribePlayerSessionsResult = DescribePlayerSessionOutcome.GetResult();
	int32 PlayerSessionCount = 0;
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(PlayerSessionCount);
	if (PlayerSessions == nullptr || PlayerSessionCount == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSession Failed (Player session is null OR Player session count is 0)");
		return;
	}

	// 3. 遍历玩家会话，比较玩家会话ID
	for (int32 i = 0; i < PlayerSessionCount; ++i)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if (!UserName.Equals(ANSI_TO_TCHAR(PlayerSession.GetPlayerId()))) continue;
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Player session status is not RESERVED, User: %s"), *UserName);
			return;
		}

		// 4. 接受玩家会话
		const Aws::GameLift::GenericOutcome& AcceptPlayerSessionGenericOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		if (!AcceptPlayerSessionGenericOutcome.IsSuccess())
		{
			OutErrorMessage = FString::Printf(TEXT("Failed to accept player session, User: %s"), *UserName);
			return;
		}
		return;
	}
	

#endif
	
	
	
}
