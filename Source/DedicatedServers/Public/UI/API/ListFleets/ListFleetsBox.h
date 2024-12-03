// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListFleetsBox.generated.h"

class UButton;
class UScrollBox;

/**
 * 列举服务器舰队列表
 */
UCLASS()
class DEDICATEDSERVERS_API UListFleetsBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ListFleets;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ListFleets;
};
