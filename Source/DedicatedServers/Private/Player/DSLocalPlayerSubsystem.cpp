// // Copyright invi1998


#include "Player/DSLocalPlayerSubsystem.h"

#include "UI/portal/PortalManager.h"


void UDSLocalPlayerSubsystem::InitializeToken(const FDSAuthenticationResult& InAuthenticationResult, UPortalManager* InPortalManager)
{
	AuthenticationResult = InAuthenticationResult;
	PortalManager = InPortalManager;

	SetRefreshTokenTimer();
}

void UDSLocalPlayerSubsystem::RefreshToken()
{
	if (PortalManager)
	{
		PortalManager->RefreshToken(AuthenticationResult.RefreshToken);
	}
}

void UDSLocalPlayerSubsystem::SetRefreshTokenTimer()
{
	if (UWorld* World = GetWorld())
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UDSLocalPlayerSubsystem::RefreshToken);
		World->GetTimerManager().SetTimer(RefreshTokenTimerHandle, TimerDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubsystem::UpdateToken(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}
