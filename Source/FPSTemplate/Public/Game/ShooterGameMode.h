// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Game/ShooterGameModeBase.h"
#include "GameLiftServerSDK.h"
#include "ShooterGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogShooterGameMode, Log, All);

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AShooterGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()
	
public:
	/**
	 * 
	 */
	AShooterGameMode();


protected:
	virtual void BeginPlay() override;
	void SetServerParameters(FServerParameters& OutServerParameters);
	void ParseCommandLinePort(int32& port);

private:
	// Process Parameters needs to remain in scope for the lifetime of the app �����̲�����Ҫ��Ӧ�ó�������������ڱ��ִ�
	FProcessParameters ProcessParameters;

	void InitGameLift();

};
