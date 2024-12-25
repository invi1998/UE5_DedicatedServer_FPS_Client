// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_LobbyGameMode.generated.h"

class UDS_GameInstanceSubsystem;
/**
 * 大厅游戏模式
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;

	FProcessParameters ProcessParameters;

	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
};
