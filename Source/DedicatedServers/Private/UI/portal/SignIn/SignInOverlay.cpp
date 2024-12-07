// // Copyright invi1998


#include "UI/portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "UI/portal/PortalManager.h"
#include "UI/portal/SignIn/ConfirmAccountPage.h"
#include "UI/portal/SignIn/SignInPage.h"
#include "UI/portal/SignIn/SignUpPage.h"
#include "UI/portal/SignIn/SuccessConfirmPage.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(PortalManagerClass, TEXT("PortalManagerClass is nullptr"));
	checkf(IsValid(JoinGameWidget), TEXT("JoinGameWidget is nullptr"));
	checkf(IsValid(JoinGameWidget->Button_JoinGame), TEXT("Button_JoinGame is nullptr"));
	
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);

	check(IsValid(Button_SignIn_Test));
	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	check(IsValid(Button_SignUp_Test));
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	check(IsValid(Button_ConfirmAccount_Test));
	Button_ConfirmAccount_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmAccountPage);
	check(IsValid(Button_SuccessConfirm_Test));
	Button_SuccessConfirm_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmPage);
	
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

void USignInOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignUpPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmAccountPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(ConfirmAccountPage));
	WidgetSwitcher->SetActiveWidget(ConfirmAccountPage);
}

void USignInOverlay::ShowSuccessConfirmPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SuccessConfirmPage));
	WidgetSwitcher->SetActiveWidget(SuccessConfirmPage);
}
