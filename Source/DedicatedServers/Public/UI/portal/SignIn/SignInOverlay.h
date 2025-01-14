// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class UButton;
class USuccessConfirmPage;
class UConfirmAccountPage;
class USignUpPage;
class USignInPage;
class UWidgetSwitcher;
class UPortalManager;
class UJoinGame;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USignInPage> SignInPage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USignUpPage> SignUpPage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UConfirmAccountPage> ConfirmAccountPage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USuccessConfirmPage> SuccessConfirmPage;
	
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	void AutoSignIn();

	UFUNCTION()
	void ShowSignInPage();

	UFUNCTION()
	void ShowSignUpPage();

	UFUNCTION()
	void ShowConfirmAccountPage();

	UFUNCTION()
	void ShowSuccessConfirmPage();

	UFUNCTION()
	void SignInButtonClicked();

	UFUNCTION()
	void SignUpButtonClicked();

	UFUNCTION()
	void ConfirmAccountButtonClicked();

	UFUNCTION()
	void ResendCodeButtonClicked();

	UFUNCTION()
	void OKButtonClicked();

	UFUNCTION()
	void OnSignUpSuccessed();

	UFUNCTION()
	void OnConfirmAccountSuccessed();

	UFUNCTION()
	void OnSignInSuccessed();

	FTimerHandle ResendCodeTimerHandle;
	int32 ResendCodeCountdown{60};
	
};
