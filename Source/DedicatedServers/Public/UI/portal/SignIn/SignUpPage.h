﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignUpPage.generated.h"

class UCheckBox;
class UTextBlock;
class UEditableTextBox;
class UButton;

/**
 * 注册页面
 */
UCLASS()
class DEDICATEDSERVERS_API USignUpPage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Username;	// 用户名输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UsernameError;	// 用户名错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Email;	// 邮箱输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_EmailError;	// 邮箱错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_FullName;	// 全名输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_FullNameError;	// 全名错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;	// 密码输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PasswordError;	// 密码错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmPassword;	// 确认密码输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ConfirmPasswordError;	// 确认密码错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckBox_ShowPassword;		// 显示密码复选框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_SignUp;	// 注册按钮

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_SignIn;	// 登录按钮

	void ResetButtonText(const FString& text = "") const;
	
	UFUNCTION()
	void UpdateStatusMessage(const FString& StatusMessage, bool bNeedRestButton);

protected:
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void ShowPassword(bool bIsChecked);
	
	UFUNCTION()
	void TextBox_Username_OnTextChanged(const FText& Text);

	UFUNCTION()
	void TextBox_Email_OnTextChanged(const FText& Text);

	UFUNCTION()
	void TextBox_FullName_OnTextChanged(const FText& Text);

	UFUNCTION()
	void TextBox_Password_OnTextChanged(const FText& Text);

	UFUNCTION()
	void TextBox_ConfirmPassword_OnTextChanged(const FText& Text);

	bool bTextBlock_UsernameIsValidate = false;
	bool bTextBlock_EmailIsValidate = false;
	bool bTextBlock_FullNameIsValidate = false;
	bool bTextBlock_PasswordIsValidate = false;
	bool bTextBlock_ConfirmPasswordIsValidate = false;
	
	
};

