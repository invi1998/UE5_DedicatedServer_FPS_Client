// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoText.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInfoText : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Info;
};
