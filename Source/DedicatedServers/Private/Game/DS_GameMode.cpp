// // Copyright invi1998


#include "DedicatedServers/Public/Game/DS_GameMode.h"

DEFINE_LOG_CATEGORY(LogDS_GameMode);

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	// GameLift Anywhere 运行在本地的服务器
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	// 进程ID
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), FPlatformProcess::GetCurrentProcessId());
	UE_LOG(LogDS_GameMode, Log, TEXT("PROCESS_ID [PID]: %s"), *OutServerParameters.m_processId)
}

void ADS_GameMode::ParseCommandLinePort(int32& port)
{
	// 通过命令行参数获取端口号，如果没有，则使用默认端口号
	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);		// 解析命令行参数
	for (const FString& Token : CommandLineTokens)
	{
		if (Token.StartsWith(TEXT("-port=")))
		{
			FString portString = Token.RightChop(6);
			port = FCString::Atoi(*portString);
			return;
		}
	}
}

void ADS_GameMode::InitGameLift()
{
	UE_LOG(LogDS_GameMode, Log, TEXT("Initializing the GameLift Server..."));

	// ��ȡGameLiftSDKģ��
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>("GameLiftServerSDK");

	// Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
	FServerParameters ServerParameters;

	SetServerParameters(ServerParameters);

	//InitSDK establishes a local connection with GameLift's agent to enable further communication.
	//Use InitSDK(serverParameters) for a GameLift Anywhere fleet.
	//Use InitSDK() for a GameLift managed EC2 fleet.
	GameLiftSdkModule->InitSDK(ServerParameters);


	//Implement callback function onStartGameSession
	//GameLift sends a game session activation request to the game server 
	//and passes a game session object with game properties and other settings. 
	//Here is where a game server takes action based on the game session object. 
	//When the game server is ready to receive incoming player connections, 
	//it invokes the server SDK call ActivateGameSession().
	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession InGameSession)
	{
		FString gameSessionId = FString(InGameSession.GetGameSessionId());
		UE_LOG(LogDS_GameMode, Log, TEXT("GameSession Initiated: %s"), *gameSessionId);
		GameLiftSdkModule->ActivateGameSession();
	};
	// 在游戏会话开始时调用
	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	//Implement callback function OnProcessTerminate
	//GameLift invokes this callback before shutting down the instance hosting this game server.
	//It gives the game server a chance to save its state, communicate with services, etc.,
	//and initiate shut down. When the game server is ready to shut down, it invokes the 
	//server SDK call ProcessEnding() to tell GameLift it is shutting down.
	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Game Server Process is terminating"));
		GameLiftSdkModule->ProcessEnding();
	};
	// 在游戏服务器进程终止时调用
	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	//Implement callback function OnHealthCheck
	//GameLift invokes this callback approximately every 60 seconds.
	//A game server might want to check the health of dependencies, etc.
	//Then it returns health status true if healthy, false otherwise.
	//The game server must respond within 60 seconds, or GameLift records 'false'.
	//In this example, the game server always reports healthy.
	auto OnHealthCheck = [=]()->bool
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Health Check is good"));
		return true;
	};
	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	//The game server gets ready to report that it is ready to host game sessions
	int32 port = FURL::UrlConfig.DefaultPort;
	ParseCommandLinePort(port);
	ProcessParameters.port = port;

	//Here, the game server tells GameLift where to find game session log files.
	//At the end of a game session, GameLift uploads everything in the specified
	//location and stores it in the cloud for access later.
	TArray<FString> LogFiles;
	LogFiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
	ProcessParameters.logParameters = LogFiles;

	//The game server calls ProcessReady() to tell GameLift it's ready to host game sessions.
	UE_LOG(LogDS_GameMode, Log, TEXT("Calling Process Ready"));
	GameLiftSdkModule->ProcessReady(ProcessParameters);
}
