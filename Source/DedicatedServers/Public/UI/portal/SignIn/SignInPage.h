// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInPage.generated.h"

class UCheckBox;
class UTextBlock;
class UEditableTextBox;
class UButton;
/**
 * 登录页面
 */
UCLASS()
class DEDICATEDSERVERS_API USignInPage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Username;	// 用户名输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UsernameError;	// 用户名错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;	// 密码输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PasswordError;	// 密码错误提示

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckBox_ShowPassword;	// 显示密码复选框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_SignIn;	// 登录按钮

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_SignUp;	// 注册按钮

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Exit;	// 退出按钮

protected:
	void ResetButtonText(const FString& text = "") const;
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void ShowPassword(bool bIsChecked);
};
