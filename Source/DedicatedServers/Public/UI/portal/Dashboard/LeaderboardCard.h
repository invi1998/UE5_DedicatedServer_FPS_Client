// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "LeaderboardCard.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULeaderboardCard : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetPlayerInfo(const FString& UserName, const FString& WinCount, const FString& Rank, const FString& SubId) const;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UserName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_WinCount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Rank;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SubId;

	
};
