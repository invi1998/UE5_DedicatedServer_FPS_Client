// // Copyright invi1998


#include "UI/portal/SignIn/SignInPage.h"

#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"

void USignInPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(IsValid(TextBox_Password))
	TextBox_Password->SetIsPassword(true);
	
	check(IsValid(CheckBox_ShowPassword))
	CheckBox_ShowPassword->OnCheckStateChanged.AddDynamic(this, &USignInPage::ShowPassword);
}

void USignInPage::ShowPassword(bool bIsChecked)
{
	if (IsValid(TextBox_Password))
	{
		TextBox_Password->SetIsPassword(!bIsChecked);
	}
}

