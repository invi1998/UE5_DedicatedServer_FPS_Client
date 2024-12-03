// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameMode.h"
#include "DS_GameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDS_GameMode, Log, All);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void SetServerParameters(FServerParameters& OutServerParameters);
	void ParseCommandLinePort(int32& port);

private:
	// Process Parameters needs to remain in scope for the lifetime of the app（进程参数需要在应用程序的生命周期内保持存活）
	FProcessParameters ProcessParameters;

	void InitGameLift();
};
