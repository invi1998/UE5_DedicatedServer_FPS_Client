// // Copyright invi1998


#include "UI/portal/Dashboard/SettingPage.h"

#include "CommonButtonBase.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/portal/PortalManager.h"

void USettingPage::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(PortalManagerClass, TEXT("PortalManagerClass is nullptr"));
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	Button_SignOut->OnClicked().AddUObject(this, &USettingPage::SignOutButtonClicked);
	PortalManager->OnSignOutCompleteDelegate.AddDynamic(this, &USettingPage::OnSignOutComplete);
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
