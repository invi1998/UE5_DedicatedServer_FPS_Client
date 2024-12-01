// // Copyright invi1998


#include "Game/ShooterGameMode.h"

DEFINE_LOG_CATEGORY(LogShooterGameMode);


AShooterGameMode::AShooterGameMode()
{
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void AShooterGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	// 从 "aws gamelift get-compute-auth-token" API 返回的 AuthToken。请注意，此令牌将在 15 分钟后过期，并需要调用 API 获取新的令牌。
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	// GameLift Anywhere 实例的主机/计算机名称。
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	// 当前进程ID
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), FPlatformProcess::GetCurrentProcessId());
	UE_LOG(LogShooterGameMode, Log, TEXT("PROCESS_ID [PID]: %s"), *OutServerParameters.m_processId)

}

void AShooterGameMode::ParseCommandLinePort(int32& port)
{
	// 从命令行参数中获取端口号，如果没有获取到，则使用虚幻引擎里设置的默认端口号
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

void AShooterGameMode::InitGameLift()
{
	UE_LOG(LogShooterGameMode, Log, TEXT("Initializing the GameLift Server..."));

	// 获取GameLiftSDK模块
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>("GameLiftServerSDK");

	// Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
	// （为GameLift Anywhere 舰队定义服务器参数。这些对于GameLift托管的EC2是不需要的。）
	//  GameLift Anywhere 是其一个特性，允许开发者在他们自己管理的基础设施上运行游戏服务器，而不是使用 AWS 管理的 EC2 实例
	//  当文档中提到 "Define the server parameters for a GameLift Anywhere fleet" 时，它指的是你需要配置和指定一系列参数，以便于在你自己控制的服务器上启动和管理一个 GameLift 服务器舰队（fleet）。这些参数可能包括但不限于：
	//  
	//  	Fleet ID 或名称：用于标识你的舰队。
	//  	服务器进程配置：这可能涉及定义每个实例应启动多少个游戏服务器进程，以及如何配置这些进程。
	//  	健康检查超时时间：定义多长时间内如果游戏服务器没有响应就被认为是不健康的。
	//  	最大并发数：设置单个实例上可以同时运行的游戏会话的最大数量。
	//  	资源管理和分配策略：如内存、CPU 的分配等。
	//  	安全配置：例如防火墙规则、端口配置等，确保游戏服务器能够正确地与玩家和其他组件通信。
	//  	日志和监控配置：指定日志记录位置、级别以及其他监控设置以跟踪性能和问题排查。
	//  	由于 GameLift Anywhere 允许你在自己的硬件上运行游戏服务器，因此你负责提供并维护该硬件，而 AWS 提供了软件和技术来帮助管理和优化这些服务器上的游戏会话。相比之下，对于 GameLift 管理的 EC2 舰队，许多这些参数由 AWS 自动处理，因为你是在使用 AWS 提供和管理的计算资源。
	// 换句话说：如果是我们本地的服务器，我们需要配置这些参数，如果是 AWS 管理的 EC2 舰队，这些参数是不需要的
	FServerParameters ServerParameters;

	// 配置服务器参数
	SetServerParameters(ServerParameters);

	//InitSDK establishes a local connection with GameLift's agent to enable further communication. （InitSDK 建立了与 GameLift 代理的本地连接，以便进一步通信。）
	//Use InitSDK(serverParameters) for a GameLift Anywhere fleet. （对于 GameLift Anywhere 舰队，请使用 InitSDK(serverParameters)。）
	//Use InitSDK() for a GameLift managed EC2 fleet.（对于 GameLift 管理的 EC2 舰队，请使用 InitSDK()。）
	GameLiftSdkModule->InitSDK(ServerParameters);


	//Implement callback function onStartGameSession（实现回调函数 onStartGameSession）
	//GameLift sends a game session activation request to the game server （GameLift 向游戏服务器发送游戏会话激活请求）
	//and passes a game session object with game properties and other settings. （并传递具有游戏属性和其他设置的游戏会话对象。）
	//Here is where a game server takes action based on the game session object. （这是游戏服务器根据游戏会话对象采取行动的地方。）
	//When the game server is ready to receive incoming player connections, （当游戏服务器准备好接收传入的玩家连接时，）
	//it invokes the server SDK call ActivateGameSession().（它调用服务器 SDK 调用 ActivateGameSession()。）
	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession InGameSession)
	{
		FString gameSessionId = FString(InGameSession.GetGameSessionId());
		UE_LOG(LogShooterGameMode, Log, TEXT("GameSession Initiated: %s"), *gameSessionId);
		GameLiftSdkModule->ActivateGameSession();
	};
	// 绑定游戏会话开始事件
	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	//Implement callback function OnProcessTerminate (实现回调函数 OnProcessTerminate)
	//GameLift invokes this callback before shutting down the instance hosting this game server. （GameLift 在关闭托管此游戏服务器的实例之前调用此回调。）
	//It gives the game server a chance to save its state, communicate with services, etc., （它给游戏服务器一个机会来保存其状态，与服务通信等。）
	//and initiate shut down. When the game server is ready to shut down, it invokes the （并启动关闭。当游戏服务器准备关闭时，它调用）
	//server SDK call ProcessEnding() to tell GameLift it is shutting down.（服务器 SDK 调用 ProcessEnding() 告诉 GameLift 它正在关闭。）
	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("Game Server Process is terminating"));
		GameLiftSdkModule->ProcessEnding();
	};
	// 绑定进程终止事件
	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	//Implement callback function OnHealthCheck（实现回调函数 OnHealthCheck）
	//GameLift invokes this callback approximately every 60 seconds.（GameLift 大约每 60 秒调用此回调。）
	//A game server might want to check the health of dependencies, etc.（游戏服务器可能想要检查依赖项的健康状况等。）
	//Then it returns health status true if healthy, false otherwise.（然后，如果健康，则返回健康状态 true，否则返回 false。）
	//The game server must respond within 60 seconds, or GameLift records 'false'.（游戏服务器必须在 60 秒内响应，否则 GameLift 记录“false”。）
	//In this example, the game server always reports healthy.（在此示例中，游戏服务器始终报告健康。）
	auto OnHealthCheck = [=]()->bool
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("Health Check is good"));
		return true;
	};
	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	//The game server gets ready to report that it is ready to host game sessions（游戏服务器准备好报告它已准备好托管游戏会话）
	// 端口号将会使用虚幻引擎里设置的默认端口号
	int32 port = FURL::UrlConfig.DefaultPort;
	ParseCommandLinePort(port);	// 获取命令行参数中的端口号（如果有的话）
	ProcessParameters.port = port;

}
