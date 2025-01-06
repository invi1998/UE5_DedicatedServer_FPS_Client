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
	ADS_LobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;	// 玩家退出
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;	// 玩家登录前

protected:
	virtual void BeginPlay() override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;		// 初始化无缝旅行玩家
	virtual void OnCountdownTimerFinished(ECountdownTimerType InTimerType) override;

	void CheckAndStartLobbyCountdown();
	void CheckAndStopLobbyCountdown();

	UPROPERTY()
	ELobbyStatus LobbyStatus;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers = 1;

	// 无缝旅行目标地图
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap;

private:
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer;
	
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	
};

