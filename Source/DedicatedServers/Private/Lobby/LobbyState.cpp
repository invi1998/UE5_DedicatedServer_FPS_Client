// // Copyright invi1998


#include "Lobby/LobbyState.h"

#include "Net/UnrealNetwork.h"


// Sets default values
ALobbyState::ALobbyState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;		// 开启复制
	bAlwaysRelevant = true;		// 始终相关
}

void ALobbyState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyState, PlayerInfoArray);		// 复制玩家信息数组
}

void ALobbyState::AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerInfoArray.AddPlayer(PlayerInfo);
}

void ALobbyState::RemovePlayerInfo(const FString& PlayerId)
{
	PlayerInfoArray.RemovePlayer(PlayerId);
}

void ALobbyState::OnRep_LobbyPlayerInfoArray()
{
	FLobbyPlayerInfoDelta Delta = ComputePlayerInfoDelta(PlayerInfoArray.Players, LastPlayerInfoArray.Players);
	for (const FLobbyPlayerInfo& PlayerInfo : Delta.AddedPlayerInfos)
	{
		OnPlayerInfoAdded.Broadcast(PlayerInfo);
	}
	for (const FLobbyPlayerInfo& PlayerInfo : Delta.RemovedPlayerInfos)
	{
		OnPlayerInfoRemoved.Broadcast(PlayerInfo);
	}
	
	LastPlayerInfoArray = PlayerInfoArray;
}

FLobbyPlayerInfoDelta ALobbyState::ComputePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& NewPlayerInfoArray, const TArray<FLobbyPlayerInfo>& OldPlayerInfoArray)
{
	// 计算玩家信息变化，我们可以知道哪些玩家被添加，哪些玩家被移除
	FLobbyPlayerInfoDelta Delta;
	TMap<FString, const FLobbyPlayerInfo*> NewPlayerInfoMap;
	TMap<FString, const FLobbyPlayerInfo*> OldPlayerInfoMap;

	for (const FLobbyPlayerInfo& PlayerInfo : NewPlayerInfoArray)
	{
		NewPlayerInfoMap.Add(PlayerInfo.PlayerSessionId, &PlayerInfo);
	}

	for (const FLobbyPlayerInfo& PlayerInfo : OldPlayerInfoArray)
	{
		OldPlayerInfoMap.Add(PlayerInfo.PlayerSessionId, &PlayerInfo);
	}

	for (const TPair<FString, const FLobbyPlayerInfo*>& Pair : OldPlayerInfoMap)
	{
		if (!NewPlayerInfoMap.Contains(Pair.Key))
		{
			Delta.RemovedPlayerInfos.Add(*Pair.Value);
		}
	}
	for (const TPair<FString, const FLobbyPlayerInfo*>& Pair : NewPlayerInfoMap)
	{
		if (!OldPlayerInfoMap.Contains(Pair.Key))
		{
			Delta.AddedPlayerInfos.Add(*Pair.Value);
		}
	}

	return Delta;
}


