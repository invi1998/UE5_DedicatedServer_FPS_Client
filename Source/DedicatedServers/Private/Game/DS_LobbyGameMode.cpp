// // Copyright invi1998


#include "Game/DS_LobbyGameMode.h"

#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

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
		LobbyStatus = ELobbyStatus::SeamlessTravel;
		const FString DestinationMapPath = DestinationMap.ToSoftObjectPath().GetAssetName();
		UE_LOG(LogDedicatedServers, Log, TEXT("Seamless Travel to %s"), *DestinationMapPath)
		if (GIsEditor)
		{
			// 编辑器模式下，直接切换地图
			UGameplayStatics::OpenLevel(GetWorld(), FName(*DestinationMapPath));
		}
		else
		{
			// 服务器模式下，无缝旅行
			GetWorld()->ServerTravel(DestinationMapPath, true, false);
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
