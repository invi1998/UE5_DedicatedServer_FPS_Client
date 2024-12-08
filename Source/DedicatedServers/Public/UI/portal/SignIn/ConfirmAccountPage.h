// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmAccountPage.generated.h"

class UTextBlock;
class UEditableTextBox;
class UButton;

/**
 * 账户确认页面
 */
UCLASS()
class DEDICATEDSERVERS_API UConfirmAccountPage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Info;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Code;		// 验证码输入框

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CodeError;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_ResendCode;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_ConfirmAccount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Back;

	UFUNCTION()
	void TextBox_Code_OnTextChanged(const FText& Text);

	void RestConfirmButtonText(const FString& text = "") const;

	void RestResendCodeButtonText(const FString& text = "") const;

	void Rest();

	UFUNCTION()
	void UpdateStatusMessage(const FString& StatusMessage, bool bNeedRestButton);

private:
	virtual void NativeConstruct() override;
	
};
