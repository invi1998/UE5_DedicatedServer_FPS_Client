// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuccessConfirmPage.generated.h"

class UButton;

/**
 * 成功确认页面
 */
UCLASS()
class DEDICATEDSERVERS_API USuccessConfirmPage : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_OK;
	
	
};
