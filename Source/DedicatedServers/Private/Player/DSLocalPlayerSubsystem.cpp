// // Copyright invi1998


#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/portal/Interfaces/PortalManagement.h"

void UDSLocalPlayerSubsystem::RefreshToken()
{
	if (PortalManagementInterface)
	{
		PortalManagementInterface->RefreshToken(AuthenticationResult.RefreshToken);
	}
}

void UDSLocalPlayerSubsystem::InitializeToken(const FDSAuthenticationResult& InAuthenticationResult, TScriptInterface<IPortalManagement> InPortalManager)
{
	AuthenticationResult = InAuthenticationResult;
	PortalManagementInterface = InPortalManager;

	SetRefreshTokenTimer();
}

void UDSLocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(PortalManagementInterface.GetObject()))
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
