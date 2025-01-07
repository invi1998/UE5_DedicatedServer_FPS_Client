// // Copyright invi1998


#include "UI/Lobby/LobbyPlayerBox.h"

#include "Components/ScrollBox.h"
#include "Game/DS_GameState.h"
#include "Lobby/LobbyState.h"
#include "UI/Lobby/PlayerLabel.h"


void ULobbyPlayerBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADS_GameState* DSGameState = GetWorld()->GetGameState<ADS_GameState>();
	if (!IsValid(DSGameState)) return;

	if (IsValid(DSGameState->LobbyState))
	{
		LobbyStateInitialized(DSGameState->LobbyState);
	}
	else
	{
		DSGameState->OnLobbyStateInitialized.AddDynamic(this, &ULobbyPlayerBox::LobbyStateInitialized);
	}
	
}

void ULobbyPlayerBox::UpdatePlayerInfo(ALobbyState* InLobbyState)
{
	if (!IsValid(InLobbyState)) return;

	ScrollBox_PlayerList->ClearChildren();

	for (const FLobbyPlayerInfo& PlayerInfo : InLobbyState->GetPlayerInfoArray())
	{
		AddPlayer(PlayerInfo);
	}
}

void ULobbyPlayerBox::AddPlayer(const FLobbyPlayerInfo& PlayerInfo)
{
	if (FindPlayerLabel(PlayerInfo.PlayerSessionId) != nullptr) return;
	
	if (!IsValid(PlayerLabelClass)) return;

	UPlayerLabel* PlayerLabel = CreateWidget<UPlayerLabel>(this, PlayerLabelClass);
	if (!IsValid(PlayerLabel)) return;
	
	FString PlayerName = PlayerInfo.UserName;
	FString PlayerSessionId = PlayerInfo.PlayerSessionId;
	PlayerLabel->SetUserName(PlayerName);
	PlayerLabel->SetPlayerSessionId(PlayerSessionId);

	ScrollBox_PlayerList->AddChild(PlayerLabel);
}

void ULobbyPlayerBox::RemovePlayer(const FLobbyPlayerInfo& PlayerInfo)
{
	UPlayerLabel* PlayerLabel = FindPlayerLabel(PlayerInfo.PlayerSessionId);
	if (IsValid(PlayerLabel))
	{
		ScrollBox_PlayerList->RemoveChild(PlayerLabel);
	}
}

void ULobbyPlayerBox::LobbyStateInitialized(ALobbyState* InLobbyState)
{
	if (!IsValid(InLobbyState)) return;
	if (!InLobbyState->OnPlayerInfoAdded.IsAlreadyBound(this, &ULobbyPlayerBox::AddPlayer))
	{
		InLobbyState->OnPlayerInfoAdded.AddDynamic(this, &ULobbyPlayerBox::AddPlayer);
	}
	if (!InLobbyState->OnPlayerInfoRemoved.IsAlreadyBound(this, &ULobbyPlayerBox::RemovePlayer))
	{
		InLobbyState->OnPlayerInfoRemoved.AddDynamic(this, &ULobbyPlayerBox::RemovePlayer);
	}
	UpdatePlayerInfo(InLobbyState);
	
}

UPlayerLabel* ULobbyPlayerBox::FindPlayerLabel(const FString& PlayerId) const
{
	for (UWidget* Widget : ScrollBox_PlayerList->GetAllChildren())
	{
		UPlayerLabel* PlayerLabel = Cast<UPlayerLabel>(Widget);
		if (IsValid(PlayerLabel) && PlayerLabel->GetPlayerSessionId().Equals(PlayerId))
		{
			return PlayerLabel;
		}
	}

	return nullptr;
}
