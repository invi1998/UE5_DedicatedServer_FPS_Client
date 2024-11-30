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

	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	// 从 "aws gamelift get-compute-auth-token" API 返回的 AuthToken。请注意，此令牌将在 15 分钟后过期，并需要调用 API 获取新的令牌。
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), ServerParameters.m_authToken))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("AUTH_TOKEN: %s"), *ServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	// GameLift Anywhere 实例的主机/计算机名称。
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), ServerParameters.m_hostId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("HOST_ID: %s"), *ServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), ServerParameters.m_fleetId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("FLEET_ID: %s"), *ServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), ServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("WEBSOCKET_URL: %s"), *ServerParameters.m_webSocketUrl)
	}

}
