// // Copyright invi1998


#include "UI/portal/SignIn/ConfirmAccountPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UConfirmAccountPage::TextBox_Code_OnTextChanged(const FText& Text)
{
	if (Text.IsEmpty() || Text.ToString().Len() != 6)
	{
		TextBlock_CodeError->SetText(FText::FromString(TEXT("验证码长度必须为 6 位")));
		Button_ConfirmAccount->SetIsEnabled(false);
		return;
	}

	const FRegexPattern CodePattern(TEXT(R"(\d{6})"));
	FRegexMatcher CodeMatcher(CodePattern, Text.ToString());
	if (!CodeMatcher.FindNext())
	{
		TextBlock_CodeError->SetText(FText::FromString(TEXT("验证码必须为数字")));
		Button_ConfirmAccount->SetIsEnabled(false);
		return;
	}
	else
	{
		TextBlock_CodeError->SetText(FText::FromString(TEXT("")));
		Button_ConfirmAccount->SetIsEnabled(true);
	}
}

void UConfirmAccountPage::RestConfirmButtonText(const FString& text) const
{
	if (Button_ConfirmAccount->GetChildrenCount() > 0)
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Button_ConfirmAccount->GetChildAt(0)))
		{
			FString buttonText = text.IsEmpty() ? HTTPStatusMessages::ConfirmAccount : text;
			TextBlock->SetText(FText::FromString(buttonText));
		}
	}
}

void UConfirmAccountPage::RestResendCodeButtonText(const FString& text) const
{
	if (Button_ResendCode->GetChildrenCount() > 0)
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Button_ResendCode->GetChildAt(0)))
		{
			FString buttonText = text.IsEmpty() ? HTTPStatusMessages::ResendCode : text;
			TextBlock->SetText(FText::FromString(buttonText));
		}
	}
}

void UConfirmAccountPage::Rest()
{
	TextBlock_Info->SetText(FText::FromString(TEXT("")));
	TextBox_Code->SetText(FText::FromString(TEXT("")));
	TextBlock_CodeError->SetText(FText::FromString(TEXT("")));
	Button_ConfirmAccount->SetIsEnabled(false);
	Button_ResendCode->SetIsEnabled(false);
	TextBox_Code->SetIsEnabled(true);
	RestConfirmButtonText();
	RestResendCodeButtonText();
}

void UConfirmAccountPage::UpdateStatusMessage(const FString& StatusMessage, bool bNeedRestButton)
{
	if (bNeedRestButton)
	{
		Button_ConfirmAccount->SetIsEnabled(true);
		TextBlock_CodeError->SetText(FText::FromString(StatusMessage));
		TextBox_Code->SetIsEnabled(true);
		RestConfirmButtonText();
	}
	else
	{
		// 禁用输入框
		TextBox_Code->SetIsEnabled(false);
		RestConfirmButtonText(StatusMessage);
	}
}

void UConfirmAccountPage::NativeConstruct()
{
	Super::NativeConstruct();

	Button_ResendCode->SetIsEnabled(false);
	Button_ConfirmAccount->SetIsEnabled(false);
	TextBox_Code->SetIsEnabled(true);

	RestConfirmButtonText();
	RestResendCodeButtonText();
}
