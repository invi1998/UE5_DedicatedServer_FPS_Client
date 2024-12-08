// // Copyright invi1998


#include "UI/portal/SignIn/SignUpPage.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void USignUpPage::ResetButtonText(const FString& text) const
{
	if (Button_SignUp->GetChildrenCount() > 0)
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Button_SignUp->GetChildAt(0)))
		{
			FString buttonText = text.IsEmpty() ? HTTPStatusMessages::SignUp : text;
			TextBlock->SetText(FText::FromString(buttonText));
		}
	}
}

void USignUpPage::UpdateStatusMessage(const FString& StatusMessage, bool bNeedRestButton)
{
	if (bNeedRestButton)
	{
		Button_SignUp->SetIsEnabled(true);
		ResetButtonText();
	}
	else
	{
		Button_SignUp->SetIsEnabled(false);
		ResetButtonText(StatusMessage);
	}
}

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox_Password->SetIsPassword(true);
	TextBox_ConfirmPassword->SetIsPassword(true);

	CheckBox_ShowPassword->OnCheckStateChanged.AddDynamic(this, &USignUpPage::ShowPassword);

	TextBox_Username->OnTextChanged.AddDynamic(this, &USignUpPage::TextBox_Username_OnTextChanged);
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::TextBox_Email_OnTextChanged);
	TextBox_FullName->OnTextChanged.AddDynamic(this, &USignUpPage::TextBox_FullName_OnTextChanged);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::TextBox_Password_OnTextChanged);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::TextBox_ConfirmPassword_OnTextChanged);

	Button_SignUp->SetIsEnabled(false);

	ResetButtonText();
}

void USignUpPage::ShowPassword(bool bIsChecked)
{
	TextBox_Password->SetIsPassword(!bIsChecked);
	TextBox_ConfirmPassword->SetIsPassword(!bIsChecked);
}

void USignUpPage::TextBox_Username_OnTextChanged(const FText& Text)
{
	// 校验用户名输入是否合法
	// 长度是否在 6-20 之间
	if (Text.IsEmpty() || Text.ToString().Len() < 6 || Text.ToString().Len() > 20)
	{
		// 显示错误提示
		TextBlock_UsernameError->SetText(FText::FromString(TEXT("用户名长度必须在 6-20 之间")));
		bTextBlock_UsernameIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
	}
	else
	{
		// 隐藏错误提示
		TextBlock_UsernameError->SetText(FText::FromString(TEXT("")));
		bTextBlock_UsernameIsValidate = true;
		Button_SignUp->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_EmailIsValidate && bTextBlock_FullNameIsValidate && bTextBlock_PasswordIsValidate && bTextBlock_ConfirmPasswordIsValidate);
	}
}

void USignUpPage::TextBox_Email_OnTextChanged(const FText& Text)
{
	// 校验邮箱输入是否合法
	// 邮箱格式是否正确
	// 定义电子邮件的正则表达式
	const FRegexPattern EmailPattern(TEXT(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"));
	FRegexMatcher EmailMatcher(EmailPattern, Text.ToString());
	if (!EmailMatcher.FindNext())
	{
		// 显示错误提示
		TextBlock_EmailError->SetText(FText::FromString(TEXT("邮箱格式不正确")));
		bTextBlock_EmailIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
	}
	else
	{
		// 隐藏错误提示
		TextBlock_EmailError->SetText(FText::FromString(TEXT("")));
		bTextBlock_EmailIsValidate = true;
		Button_SignUp->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_EmailIsValidate && bTextBlock_FullNameIsValidate && bTextBlock_PasswordIsValidate && bTextBlock_ConfirmPasswordIsValidate);
	}
}

void USignUpPage::TextBox_FullName_OnTextChanged(const FText& Text)
{
	// 校验用户名输入是否合法
	// 长度是否在 6-20 之间
	if (Text.IsEmpty() || Text.ToString().Len() < 6 || Text.ToString().Len() > 20)
	{
		// 显示错误提示
		TextBlock_FullNameError->SetText(FText::FromString(TEXT("用户名长度必须在 6-20 之间")));
		bTextBlock_FullNameIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
	}
	else
	{
		// 隐藏错误提示
		TextBlock_FullNameError->SetText(FText::FromString(TEXT("")));
		bTextBlock_FullNameIsValidate = true;
		Button_SignUp->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_EmailIsValidate && bTextBlock_FullNameIsValidate && bTextBlock_PasswordIsValidate && bTextBlock_ConfirmPasswordIsValidate);
	}
}

void USignUpPage::TextBox_Password_OnTextChanged(const FText& Text)
{
	// Password must be at least 8 characters
	if (Text.IsEmpty() || Text.ToString().Len() < 8)
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码长度必须至少 8 个字符")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
		return;
	}
	// Use a number
	const FRegexPattern NumberPattern(TEXT(R"(\d+)"));
	if (!FRegexMatcher(NumberPattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含数字")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
		return;
	}
	// Use a lowercase letter
	const FRegexPattern LowercasePattern(TEXT(R"([a-z]+)"));
	if (!FRegexMatcher(LowercasePattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含小写字母")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
		return;
	}
	// Use an uppercase letter
	const FRegexPattern UppercasePattern(TEXT(R"([A-Z]+)"));
	if (!FRegexMatcher(UppercasePattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含大写字母")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
		return;
	}
	// Use a symbol
	const FRegexPattern SymbolPattern(TEXT(R"([!@#$%^&*()_+{}|:"<>?])"));
	if (!FRegexMatcher(SymbolPattern, Text.ToString()).FindNext())
	{
		// 显示错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("密码必须包含特殊字符")));
		bTextBlock_PasswordIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
		return;
	}
	else
	{
		// 隐藏错误提示
		TextBlock_PasswordError->SetText(FText::FromString(TEXT("")));
		bTextBlock_PasswordIsValidate = true;
		Button_SignUp->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_EmailIsValidate && bTextBlock_FullNameIsValidate && bTextBlock_PasswordIsValidate && bTextBlock_ConfirmPasswordIsValidate);
	}
	
}

void USignUpPage::TextBox_ConfirmPassword_OnTextChanged(const FText& Text)
{
	// 校验两次密码输入是否一致
	if (Text.IsEmpty() || Text.ToString() != TextBox_Password->GetText().ToString())
	{
		// 显示错误提示
		TextBlock_ConfirmPasswordError->SetText(FText::FromString(TEXT("两次密码输入不一致")));
		bTextBlock_ConfirmPasswordIsValidate = false;
		Button_SignUp->SetIsEnabled(false);
	}
	else
	{
		// 隐藏错误提示
		TextBlock_ConfirmPasswordError->SetText(FText::FromString(TEXT("")));
		bTextBlock_ConfirmPasswordIsValidate = true;
		Button_SignUp->SetIsEnabled(bTextBlock_UsernameIsValidate && bTextBlock_EmailIsValidate && bTextBlock_FullNameIsValidate && bTextBlock_PasswordIsValidate && bTextBlock_ConfirmPasswordIsValidate);
	}
}
