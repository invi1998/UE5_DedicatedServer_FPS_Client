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
	// Process Parameters needs to remain in scope for the lifetime of the app （进程参数需要在应用程序的生命周期内保持存活）
	FProcessParameters ProcessParameters;

	void InitGameLift();

};
