// // Copyright invi1998


#include "UI/portal/Dashboard/SettingPage.h"

#include "CommonButtonBase.h"
#include "Components/TextBlock.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/portal/PortalManager.h"

void USettingPage::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(PortalManagerClass, TEXT("PortalManagerClass is nullptr"));
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	if (IsValid(PortalManager))
	{
		Button_SignOut->OnClicked().AddUObject(this, &USettingPage::SignOutButtonClicked);
		PortalManager->OnSignOutCompleteDelegate.AddDynamic(this, &USettingPage::OnSignOutComplete);

		if (APlayerController* PlayerController = GetOwningPlayer())
		{
			if (UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>())
			{
				TextBlock_Username->SetText(FText::FromString(LocalPlayerSubsystem->UserName));
				TextBlock_Email->SetText(FText::FromString(EncryptEmail(LocalPlayerSubsystem->Email)));
			}
		}
	}
}

void USettingPage::SignOutButtonClicked()
{
	check(PortalManager);
	Button_SignOut->SetIsEnabled(false);
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>())
		{
			PortalManager->SignOut(LocalPlayerSubsystem->GetAuthenticationResult().AccessToken);
		}
	}
}

void USettingPage::OnSignOutComplete()
{
	Button_SignOut->SetIsEnabled(true);
	
}

FString USettingPage::EncryptEmail(const FString& Email) const
{
	// Email 加密显示（只显示前2位和@符号和@符号后一位），其余用*代替
	if (Email.Len() <= 3) // 如果email长度小于等于3，则无法进行处理，直接返回原值或采取其他措施
	{
		return Email;
	}

	int32 AtIndex = Email.Find(TEXT("@"));
	if (AtIndex == INDEX_NONE || AtIndex < 2) // 如果没有找到@或者@前面的字符少于2个，则无法处理，直接返回原值或采取其他措施
	{
		return Email;
	}

	// 创建一个新的字符串来构建加密后的结果
	FString EncryptedEmail;

	// 添加前两位字符
	EncryptedEmail += Email.Mid(0, 2);

	// 添加*来替代中间的字符
	for (int32 i = 2; i < AtIndex; ++i)
	{
		EncryptedEmail += TEXT("*");
	}

	// 添加@符号和它后面的字符
	EncryptedEmail += Email.Mid(AtIndex, 2);

	// 添加*来替代@符号后面的其余字符
	for (int32 i = AtIndex + 2; i < Email.Len(); ++i)
	{
		EncryptedEmail += TEXT("*");
	}

	return EncryptedEmail;
}

