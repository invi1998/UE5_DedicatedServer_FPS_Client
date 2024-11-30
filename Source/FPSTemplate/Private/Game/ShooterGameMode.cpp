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

	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	// �� "aws gamelift get-compute-auth-token" API ���ص� AuthToken����ע�⣬�����ƽ��� 15 ���Ӻ���ڣ�����Ҫ���� API ��ȡ�µ����ơ�
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), ServerParameters.m_authToken))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("AUTH_TOKEN: %s"), *ServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	// GameLift Anywhere ʵ��������/��������ơ�
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
