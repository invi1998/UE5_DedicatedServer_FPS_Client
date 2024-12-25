// // Copyright invi1998


#include "Game/DS_LobbyGameMode.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	LobbyStatus = ELobbyStatus::WaitingForPlayers;
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
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
	UE_LOG(LogDS_GameMode, Log, TEXT("PROCESS_ID [PID]: %s"), *OutServerParameters.m_processId)
}
