// // Copyright invi1998


#include "Game/DS_GameInstanceSubsystem.h"

#include "DedicatedServers/DedicatedServers.h"

UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
	bGameLiftInitialized = false;
}

void UDS_GameInstanceSubsystem::InitGameLift(const FServerParameters& ServerParameters)
{
	if (bGameLiftInitialized) return;

#if WITH_GAMELIFT
	UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Server..."));

	// 初始化GameLift服务器
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>("GameLiftServerSDK");
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
		UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Initiated: %s"), *gameSessionId);
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
		UE_LOG(LogDedicatedServers, Log, TEXT("Game Server Process is terminating"));
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
		UE_LOG(LogDedicatedServers, Log, TEXT("Health Check is good"));
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
	UE_LOG(LogDedicatedServers, Log, TEXT("Calling Process Ready"));
	GameLiftSdkModule->ProcessReady(ProcessParameters);
#endif

	bGameLiftInitialized = true;
}

void UDS_GameInstanceSubsystem::ParseCommandLinePort(int32& port)
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
			if (portString.IsNumeric())
			{
				LexFromString(port, *portString);
			}
			else
			{
				UE_LOG(LogDedicatedServers, Warning, TEXT("Invalid port number: %s"), *portString);
			}
			return;
		}
	}
}
