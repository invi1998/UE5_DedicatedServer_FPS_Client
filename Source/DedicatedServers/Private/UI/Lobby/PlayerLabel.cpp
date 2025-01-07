// // Copyright invi1998


#include "UI/Lobby/PlayerLabel.h"

#include "Components/TextBlock.h"

void UPlayerLabel::SetUserName(const FString& InUserName)
{
	TextBlock_UserName->SetText(FText::FromString(InUserName));
}

void UPlayerLabel::SetPlayerSessionId(const FString& InPlayerSessionId)
{
	TextBlock_PlayerSessionId->SetText(FText::FromString(InPlayerSessionId));
}

FString UPlayerLabel::GetPlayerSessionId() const
{
	return TextBlock_PlayerSessionId->GetText().ToString();
}
