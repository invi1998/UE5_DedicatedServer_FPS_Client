// // Copyright invi1998


#include "UI/portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "UI/portal/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(PortalManagerClass, TEXT("PortalManagerClass is nullptr"));
	checkf(IsValid(JoinGameWidget), TEXT("JoinGameWidget is nullptr"));
	checkf(IsValid(JoinGameWidget->Button_JoinGame), TEXT("Button_JoinGame is nullptr"));
	
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);
}

void USignInOverlay::OnJoinGameButtonClicked()
{
	checkf(IsValid(PortalManager), TEXT("PortalManager is nullptr"));
	checkf(IsValid(JoinGameWidget), TEXT("JoinGameWidget is nullptr"));
	checkf(IsValid(JoinGameWidget->Button_JoinGame), TEXT("Button_JoinGame is nullptr"));
	
	PortalManager->OnJoinGameSessionMessage.AddDynamic(this, &USignInOverlay::UpdateJoinGameStatusMessage);
	PortalManager->JoinGameSession();

	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
}

void USignInOverlay::UpdateJoinGameStatusMessage(const FString& Message, bool bNeedRestJoinGameButton)
{
	checkf(IsValid(JoinGameWidget), TEXT("JoinGameWidget is nullptr"));
	checkf(IsValid(JoinGameWidget->Button_JoinGame), TEXT("Button_JoinGame is nullptr"));
	JoinGameWidget->SetStatusMessage(Message);

	if (bNeedRestJoinGameButton)
	{
		JoinGameWidget->Button_JoinGame->SetIsEnabled(true);
	}
	
}
