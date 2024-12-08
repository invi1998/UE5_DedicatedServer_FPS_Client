// // Copyright invi1998


#include "UI/portal/SignIn/SignInPage.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void USignInPage::ResetButtonText(const FString& text) const
{
	if (Button_SignIn->GetChildrenCount() > 0)
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Button_SignUp->GetChildAt(0)))
		{
			FString buttonText = text.IsEmpty() ? HTTPStatusMessages::SignIn : text;
			TextBlock->SetText(FText::FromString(buttonText));
		}
	}
}

void USignInPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(IsValid(TextBox_Password))
	TextBox_Password->SetIsPassword(true);
	
	check(IsValid(CheckBox_ShowPassword))
	CheckBox_ShowPassword->OnCheckStateChanged.AddDynamic(this, &USignInPage::ShowPassword);

	Button_SignIn->SetIsEnabled(false);
	ResetButtonText();
	
}

void USignInPage::ShowPassword(bool bIsChecked)
{
	if (IsValid(TextBox_Password))
	{
		TextBox_Password->SetIsPassword(!bIsChecked);
	}
}

