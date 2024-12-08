// // Copyright invi1998


#include "UI/portal/PortalManager.h"

#include "Kismet/GameplayStatics.h"

void UPortalManager::QuitGame()
{
	if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}

void UPortalManager::SignIn(const FString& Username, const FString& Password)
{
}

void UPortalManager::SignUp(const FString& Username, const FString& Email, const FString& FullName, const FString& Password)
{
}

void UPortalManager::ConfirmAccount(const FString& Code)
{
}

void UPortalManager::ResendCode()
{
}


