// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingPage.generated.h"

class UPortalManager;
class UCommonButtonBase;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USettingPage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_SignOut;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	void SignOutButtonClicked();

	UFUNCTION()
	void OnSignOutComplete();

	// Email 加密显示（只显示前2位和@符号和@符号后一位），其余用*代替
	FString EncryptEmail(const FString& Email) const;
};
