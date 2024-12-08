// // Copyright invi1998


#include "UI/portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
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
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	Button_ConfirmAccount_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmAccountPage);
	Button_SuccessConfirm_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmPage);


	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::SignInButtonClicked);
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	SignInPage->Button_Exit->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);
	
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::SignUpButtonClicked);
	SignUpPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	PortalManager->OnSignUpStatusMessageDelegate.AddDynamic(SignUpPage, &USignUpPage::UpdateStatusMessage);
	
	ConfirmAccountPage->Button_ConfirmAccount->OnClicked.AddDynamic(this, &USignInOverlay::ConfirmAccountButtonClicked);
	ConfirmAccountPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	ConfirmAccountPage->Button_ResendCode->OnClicked.AddDynamic(this, &USignInOverlay::ResendCodeButtonClicked);
	
	SuccessConfirmPage->Button_OK->OnClicked.AddDynamic(this, &USignInOverlay::OKButtonClicked);
}

void USignInOverlay::ShowSignInPage()
{
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage()
{
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmAccountPage()
{
	WidgetSwitcher->SetActiveWidget(ConfirmAccountPage);
}

void USignInOverlay::ShowSuccessConfirmPage()
{
	WidgetSwitcher->SetActiveWidget(SuccessConfirmPage);
}

void USignInOverlay::SignInButtonClicked()
{
	const FString Username = SignInPage->TextBox_Username->GetText().ToString();
	const FString Password = SignInPage->TextBox_Password->GetText().ToString();
	PortalManager->SignIn(Username, Password);
}

void USignInOverlay::SignUpButtonClicked()
{
	const FString Username = SignUpPage->TextBox_Username->GetText().ToString();
	const FString Email = SignUpPage->TextBox_Email->GetText().ToString();
	const FString FullName = SignUpPage->TextBox_FullName->GetText().ToString();
	const FString Password = SignUpPage->TextBox_Password->GetText().ToString();
	PortalManager->SignUp(Username, Email, FullName, Password);
}

void USignInOverlay::ConfirmAccountButtonClicked()
{
	const FString Code = ConfirmAccountPage->TextBox_Code->GetText().ToString();
	const FString Username = SignUpPage->TextBox_Username->GetText().ToString();
	PortalManager->ConfirmAccount(Code, Username);
}

void USignInOverlay::ResendCodeButtonClicked()
{
	PortalManager->ResendCode();
}

void USignInOverlay::OKButtonClicked()
{
	ShowSignInPage();
}
