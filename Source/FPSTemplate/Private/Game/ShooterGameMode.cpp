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
	// �� "aws gamelift get-compute-auth-token" API ���ص� AuthToken����ע�⣬�����ƽ��� 15 ���Ӻ���ڣ�����Ҫ���� API ��ȡ�µ����ơ�
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	// GameLift Anywhere ʵ��������/��������ơ�
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

	// ��ǰ����ID
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), FPlatformProcess::GetCurrentProcessId());
	UE_LOG(LogShooterGameMode, Log, TEXT("PROCESS_ID [PID]: %s"), *OutServerParameters.m_processId)

}

void AShooterGameMode::ParseCommandLinePort(int32& port)
{
	// �������в����л�ȡ�˿ںţ����û�л�ȡ������ʹ��������������õ�Ĭ�϶˿ں�
	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);		// ���������в���
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

	// ��ȡGameLiftSDKģ��
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>("GameLiftServerSDK");

	// Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
	// ��ΪGameLift Anywhere ���Ӷ����������������Щ����GameLift�йܵ�EC2�ǲ���Ҫ�ġ���
	//  GameLift Anywhere ����һ�����ԣ����������������Լ�����Ļ�����ʩ��������Ϸ��������������ʹ�� AWS ����� EC2 ʵ��
	//  ���ĵ����ᵽ "Define the server parameters for a GameLift Anywhere fleet" ʱ����ָ��������Ҫ���ú�ָ��һϵ�в������Ա��������Լ����Ƶķ������������͹���һ�� GameLift ���������ӣ�fleet������Щ�������ܰ����������ڣ�
	//  
	//  	Fleet ID �����ƣ����ڱ�ʶ��Ľ��ӡ�
	//  	�������������ã�������漰����ÿ��ʵ��Ӧ�������ٸ���Ϸ���������̣��Լ����������Щ���̡�
	//  	������鳬ʱʱ�䣺����೤ʱ���������Ϸ������û����Ӧ�ͱ���Ϊ�ǲ������ġ�
	//  	��󲢷��������õ���ʵ���Ͽ���ͬʱ���е���Ϸ�Ự�����������
	//  	��Դ����ͷ�����ԣ����ڴ桢CPU �ķ���ȡ�
	//  	��ȫ���ã��������ǽ���򡢶˿����õȣ�ȷ����Ϸ�������ܹ���ȷ������Һ��������ͨ�š�
	//  	��־�ͼ�����ã�ָ����־��¼λ�á������Լ�������������Ը������ܺ������Ų顣
	//  	���� GameLift Anywhere ���������Լ���Ӳ����������Ϸ������������㸺���ṩ��ά����Ӳ������ AWS �ṩ������ͼ���������������Ż���Щ�������ϵ���Ϸ�Ự�����֮�£����� GameLift ����� EC2 ���ӣ������Щ������ AWS �Զ�������Ϊ������ʹ�� AWS �ṩ�͹���ļ�����Դ��
	// ���仰˵����������Ǳ��صķ�������������Ҫ������Щ����������� AWS ����� EC2 ���ӣ���Щ�����ǲ���Ҫ��
	FServerParameters ServerParameters;

	// ���÷���������
	SetServerParameters(ServerParameters);

	//InitSDK establishes a local connection with GameLift's agent to enable further communication. ��InitSDK �������� GameLift ����ı������ӣ��Ա��һ��ͨ�š���
	//Use InitSDK(serverParameters) for a GameLift Anywhere fleet. ������ GameLift Anywhere ���ӣ���ʹ�� InitSDK(serverParameters)����
	//Use InitSDK() for a GameLift managed EC2 fleet.������ GameLift ����� EC2 ���ӣ���ʹ�� InitSDK()����
	GameLiftSdkModule->InitSDK(ServerParameters);


	//Implement callback function onStartGameSession��ʵ�ֻص����� onStartGameSession��
	//GameLift sends a game session activation request to the game server ��GameLift ����Ϸ������������Ϸ�Ự��������
	//and passes a game session object with game properties and other settings. �������ݾ�����Ϸ���Ժ��������õ���Ϸ�Ự���󡣣�
	//Here is where a game server takes action based on the game session object. ��������Ϸ������������Ϸ�Ự�����ȡ�ж��ĵط�����
	//When the game server is ready to receive incoming player connections, ������Ϸ������׼���ý��մ�����������ʱ����
	//it invokes the server SDK call ActivateGameSession().�������÷����� SDK ���� ActivateGameSession()����
	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession InGameSession)
	{
		FString gameSessionId = FString(InGameSession.GetGameSessionId());
		UE_LOG(LogShooterGameMode, Log, TEXT("GameSession Initiated: %s"), *gameSessionId);
		GameLiftSdkModule->ActivateGameSession();
	};
	// ����Ϸ�Ự��ʼ�¼�
	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	//Implement callback function OnProcessTerminate (ʵ�ֻص����� OnProcessTerminate)
	//GameLift invokes this callback before shutting down the instance hosting this game server. ��GameLift �ڹر��йܴ���Ϸ��������ʵ��֮ǰ���ô˻ص�����
	//It gives the game server a chance to save its state, communicate with services, etc., ��������Ϸ������һ��������������״̬�������ͨ�ŵȡ���
	//and initiate shut down. When the game server is ready to shut down, it invokes the ���������رա�����Ϸ������׼���ر�ʱ�������ã�
	//server SDK call ProcessEnding() to tell GameLift it is shutting down.�������� SDK ���� ProcessEnding() ���� GameLift �����ڹرա���
	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("Game Server Process is terminating"));
		GameLiftSdkModule->ProcessEnding();
	};
	// �󶨽�����ֹ�¼�
	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	//Implement callback function OnHealthCheck��ʵ�ֻص����� OnHealthCheck��
	//GameLift invokes this callback approximately every 60 seconds.��GameLift ��Լÿ 60 ����ô˻ص�����
	//A game server might want to check the health of dependencies, etc.����Ϸ������������Ҫ���������Ľ���״���ȡ���
	//Then it returns health status true if healthy, false otherwise.��Ȼ������������򷵻ؽ���״̬ true�����򷵻� false����
	//The game server must respond within 60 seconds, or GameLift records 'false'.����Ϸ������������ 60 ������Ӧ������ GameLift ��¼��false������
	//In this example, the game server always reports healthy.���ڴ�ʾ���У���Ϸ������ʼ�ձ��潡������
	auto OnHealthCheck = [=]()->bool
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("Health Check is good"));
		return true;
	};
	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	//The game server gets ready to report that it is ready to host game sessions����Ϸ������׼���ñ�������׼�����й���Ϸ�Ự��
	// �˿ںŽ���ʹ��������������õ�Ĭ�϶˿ں�
	int32 port = FURL::UrlConfig.DefaultPort;
	ParseCommandLinePort(port);	// ��ȡ�����в����еĶ˿ںţ�����еĻ���
	ProcessParameters.port = port;

}
