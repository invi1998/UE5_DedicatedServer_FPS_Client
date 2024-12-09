// // Copyright invi1998


#include "Player/DSLocalPlayerSubsystem.h"


void UDSLocalPlayerSubsystem::InitializeToken(const FDSAuthenticationResult& InAuthenticationResult, UPortalManager* InPortalManager)
{
	AuthenticationResult = InAuthenticationResult;
	PortalManager = InPortalManager;
}
