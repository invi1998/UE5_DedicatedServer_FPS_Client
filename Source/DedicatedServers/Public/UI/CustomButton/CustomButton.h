// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "CustomButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonClickedEvent);

/**
 * 自定义按钮
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UCustomButton : public UButton
{
	GENERATED_BODY()

public:
	
};
