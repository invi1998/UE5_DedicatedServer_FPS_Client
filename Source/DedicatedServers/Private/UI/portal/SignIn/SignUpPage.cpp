// // Copyright invi1998


#include "UI/portal/SignIn/SignUpPage.h"

#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	check(IsValid(TextBox_Password));
	check(IsValid(TextBox_ConfirmPassword));
	TextBox_Password->SetIsPassword(true);
	TextBox_ConfirmPassword->SetIsPassword(true);

	check(IsValid(CheckBox_ShowPassword));
	CheckBox_ShowPassword->OnCheckStateChanged.AddDynamic(this, &USignUpPage::ShowPassword);
}

void USignUpPage::ShowPassword(bool bIsChecked)
{
	TextBox_Password->SetIsPassword(!bIsChecked);
	TextBox_ConfirmPassword->SetIsPassword(!bIsChecked);
}
