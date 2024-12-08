// // Copyright invi1998


#include "UI/portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
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
	
	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::SignInButtonClicked);
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	SignInPage->Button_Exit->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);
	PortalManager->OnSignInStatusMessageDelegate.AddDynamic(SignInPage, &USignInPage::UpdateStatusMessage);
	PortalManager->OnSignInCompleteDelegate.AddDynamic(this, &USignInOverlay::OnSignInSuccessed);
	
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::SignUpButtonClicked);
	SignUpPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	PortalManager->OnSignUpStatusMessageDelegate.AddDynamic(SignUpPage, &USignUpPage::UpdateStatusMessage);
	PortalManager->OnSignUpCompleteDelegate.AddDynamic(this, &USignInOverlay::OnSignUpSuccessed);
	
	ConfirmAccountPage->Button_ConfirmAccount->OnClicked.AddDynamic(this, &USignInOverlay::ConfirmAccountButtonClicked);
	ConfirmAccountPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	ConfirmAccountPage->Button_ResendCode->OnClicked.AddDynamic(this, &USignInOverlay::ResendCodeButtonClicked);
	PortalManager->OnConfirmAccountStatusMessageDelegate.AddDynamic(ConfirmAccountPage, &UConfirmAccountPage::UpdateStatusMessage);
	PortalManager->OnConfirmAccountCompleteDelegate.AddDynamic(this, &USignInOverlay::OnConfirmAccountSuccessed);
	
	SuccessConfirmPage->Button_OK->OnClicked.AddDynamic(this, &USignInOverlay::OKButtonClicked);

	
}

void USignInOverlay::NativeDestruct()
{
	Super::NativeDestruct();

	// 清理定时器
	if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
	{
		LocalPlayerController->GetWorldTimerManager().ClearTimer(ResendCodeTimerHandle);
	}
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
	const FString Username = PortalManager->LastSignUpUsername;
	PortalManager->ConfirmAccount(Code, Username);
}

void USignInOverlay::ResendCodeButtonClicked()
{
	PortalManager->ResendCode();
}

void USignInOverlay::OKButtonClicked()
{
	SignInPage->TextBox_Username->SetText(FText::FromString(PortalManager->LastSignUpUsername));
	SignInPage->TextBox_Password->SetText(FText::FromString(PortalManager->LastSignUpPassword));
	ShowSignInPage();
	SignInPage->Button_SignIn->SetIsEnabled(true);
}

void USignInOverlay::OnSignUpSuccessed()
{
	ShowConfirmAccountPage();
	
	ConfirmAccountPage->Rest();

	// 组装邮件发送成功提示，格式如下：
	// We have sent a code in an Email message to 2***@q***. To confirm your account, enter your code.
	const FString DestinationEmail = PortalManager->LastSignUpResponse.CodeDeliveryDetails.Destination;
	const FString Info = FString::Printf(TEXT("We have sent a code in an Email message to %s. To confirm your account, enter your code."), *DestinationEmail); 
	// 中文提示
	const FString InfoCN = FString::Printf(TEXT("我们已经向邮件 %s 发送了一条验证码消息。要确认您的帐户，请输入代码。"), *DestinationEmail);
	// 中英文提示
	const FString InfoFinal = FString::Printf(TEXT("%s\n\n%s"), *InfoCN, *Info);
	
	ConfirmAccountPage->TextBlock_Info->SetText(FText::FromString(InfoFinal));

	// 每1分钟，允许重新发送验证码，然后每秒更新一次重发按钮的文本
	ConfirmAccountPage->Button_ResendCode->SetIsEnabled(false);
	FTimerDelegate ConfirmTextDelegate = FTimerDelegate::CreateLambda([this]()
	{
		if (ResendCodeCountdown > 0)
		{
			ConfirmAccountPage->RestResendCodeButtonText(FString::Printf(TEXT("重新发送验证码（Resend code）%d"), ResendCodeCountdown));
			ResendCodeCountdown--;
		}
		else
		{
			ConfirmAccountPage->Button_ResendCode->SetIsEnabled(true);
			ConfirmAccountPage->RestResendCodeButtonText();
			ResendCodeCountdown = 60;
			// 停止定时器
			if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
			{
				LocalPlayerController->GetWorldTimerManager().ClearTimer(ResendCodeTimerHandle);
			}
		}
	});
	if (APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
	{
		if (!LocalPlayerController->GetWorldTimerManager().IsTimerActive(ResendCodeTimerHandle))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(ResendCodeTimerHandle, ConfirmTextDelegate, 1.f, true);
		}
	}
}

void USignInOverlay::OnConfirmAccountSuccessed()
{
	ShowSuccessConfirmPage();
}

void USignInOverlay::OnSignInSuccessed()
{
	// 进入游戏

}
