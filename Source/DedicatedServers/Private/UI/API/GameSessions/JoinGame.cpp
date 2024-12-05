// // Copyright invi1998


#include "UI/API/GameSessions/JoinGame.h"

#include "Components/TextBlock.h"

void UJoinGame::SetStatusMessage(const FString& Message) const
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
}
