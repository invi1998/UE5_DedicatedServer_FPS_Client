// // Copyright invi1998


#include "UI/portal/SignIn/SignInPage.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void USignInPage::UpdateStatusMessage(const FString& StatusMessage, bool bNeedRestButton)
{
	if (bNeedRestButton)
	{
		Button_SignIn->SetIsEnabled(true);
		ResetButtonText();
	}
	else
	{
		Button_SignIn->SetIsEnabled(false);
		ResetButtonText(StatusMessage);
	}
}

void USignInPage::ResetButtonText(const FString& text) const
{
	if (Button_SignIn->GetChildrenCount() > 0)
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Button_SignIn->GetChildAt(0)))
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

	TextBox_Username->OnTextChanged.AddDynamic(this, &USignInPage::TextBox_Username_OnTextChanged);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignInPage::TextBox_Password_OnTextChanged);

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

void USignInPage::TextBox_Username_OnTextChanged(const FText& Text)
{
	// 用户名长度是否在 6-20 之间
	if (Text.IsEmpty() || Text.ToString().Len() < 6 || Text.ToString().Len() > 20)
	{
		// 显示错误提示
		if (IsValid(TextBlock_UsernameError))
		{
			TextBlock_UsernameError->SetText(FText::FromString(TEXT("用户名长度必须在 6-20 之间")));
		}
		bTextBlock_UsernameIsValidate = false;
		Button_SignIn->SetIsEnabled(false);
	}
	else
	{
		// 隐藏错误提示
		if (IsValid(TextBlock_UsernameError))
		{
			TextBlock_UsernameError->SetText(FText::FromString(TEXT("")));
		}
		bTextBlock_UsernameIsValidate = true;
		Button_SignIn->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_PasswordIsValidate);
	}
}

void USignInPage::TextBox_Password_OnTextChanged(const FText& Text)
{
	// Password must be at least 8 characters
	if (Text.IsEmpty() || Text.ToString().Len() < 8)
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码长度必须至少 8 个字符")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignIn->SetIsEnabled(false);
		return;
	}
	// Use a number
	const FRegexPattern NumberPattern(TEXT(R"(\d+)"));
	if (!FRegexMatcher(NumberPattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含数字")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignIn->SetIsEnabled(false);
		return;
	}
	// Use a lowercase letter
	const FRegexPattern LowercasePattern(TEXT(R"([a-z]+)"));
	if (!FRegexMatcher(LowercasePattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含小写字母")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignIn->SetIsEnabled(false);
		return;
	}
	// Use an uppercase letter
	const FRegexPattern UppercasePattern(TEXT(R"([A-Z]+)"));
	if (!FRegexMatcher(UppercasePattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含大写字母")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignIn->SetIsEnabled(false);
		return;
	}
	// Use a symbol
	const FRegexPattern SymbolPattern(TEXT(R"([!@#$%^&*()_+{}|:"<>?])"));
	if (!FRegexMatcher(SymbolPattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含特殊字符")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignIn->SetIsEnabled(false);
		return;
	}
	else
	{
		// 隐藏错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("")));
		bTextBlock_PasswordIsValidate = true;
		Button_SignIn->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_PasswordIsValidate);
	}
}

