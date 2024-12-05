// // Copyright invi1998


#include "UI/portal/PortalManager.h"

void UPortalManager::JoinGameSession()
{
	OnJoinGameSessionMessage.Broadcast(TEXT("Searching for game session..."));
}
