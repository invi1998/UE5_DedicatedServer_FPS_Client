// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLoading.generated.h"

class UInfoText;
class UVerticalBox;
class UCommonButtonBase;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameLoading : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> Button_Loading;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInfoText> InfoTextClass;

	void AddLoadingText(const FString& Text);
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_LoadingTextBox;	// 垂直框, 用于显示加载文本
	
};
