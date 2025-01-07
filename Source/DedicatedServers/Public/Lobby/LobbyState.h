// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerInfo.h"
#include "GameFramework/Info.h"
#include "LobbyState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChanged, const FLobbyPlayerInfo&, PlayerInfo);


USTRUCT()
struct FLobbyPlayerInfoDelta
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLobbyPlayerInfo> AddedPlayerInfos;
	
	UPROPERTY()
	TArray<FLobbyPlayerInfo> RemovedPlayerInfos;
};

UCLASS()
class DEDICATEDSERVERS_API ALobbyState : public AInfo
{
	GENERATED_BODY()

public:
	ALobbyState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoAdded;	// 玩家添加委托

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoRemoved;	// 玩家移除委托

	void AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo);
	void RemovePlayerInfo(const FString& PlayerId);
	TArray<FLobbyPlayerInfo>& GetPlayerInfoArray();
	
protected:
	UFUNCTION()
	void OnRep_LobbyPlayerInfoArray();

private:
	UPROPERTY(ReplicatedUsing = OnRep_LobbyPlayerInfoArray)
	FLobbyPlayerInfoArray PlayerInfoArray;

	// 因为我们需要差异化比较，但是对于数组我们无法像普通类型的复制变量一样传递该类型参数的旧值，所以我们需要一个临时数组来存储上一次的玩家信息
	UPROPERTY()
	FLobbyPlayerInfoArray LastPlayerInfoArray;

	static FLobbyPlayerInfoDelta ComputePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& NewPlayerInfoArray, const TArray<FLobbyPlayerInfo>& OldPlayerInfoArray);
};
