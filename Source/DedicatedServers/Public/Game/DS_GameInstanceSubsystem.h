// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameLiftServerSDKModels.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DS_GameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDS_GameInstanceSubsystem();
	void InitGameLift(const FServerParameters& ServerParameters);

	UPROPERTY(BlueprintReadOnly, Category = "GameLift")
	bool bGameLiftInitialized = false;

private:
	// Process Parameters needs to remain in scope for the lifetime of the app（进程参数需要在应用程序的生命周期内保持存活）
	FProcessParameters ProcessParameters;

	void ParseCommandLinePort(int32& port);
};
