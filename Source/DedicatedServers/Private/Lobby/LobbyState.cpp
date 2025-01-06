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
}


