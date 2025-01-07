// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLabel.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPlayerLabel : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUserName(const FString& InUserName);
	void SetPlayerSessionId(const FString& InPlayerSessionId);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerSessionId;
	
};
