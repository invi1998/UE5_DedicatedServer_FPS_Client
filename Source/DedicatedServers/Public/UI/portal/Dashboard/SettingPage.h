// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingPage.generated.h"

class UPortalManager;
class UCommonButtonBase;
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

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	void SignOutButtonClicked();

	UFUNCTION()
	void OnSignOutComplete();
};
