// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Leaderboard.generated.h"

class ULeaderboardCard;
class UScrollBox;
class UTextBlock;
struct FDSLeaderboardItem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULeaderboard : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Leaderboard;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;

	UPROPERTY(EditDefaultsOnly, Category = "Leaderboard")
	TSubclassOf<ULeaderboardCard> LeaderboardCardClass;

	void PopulateLeaderboard(TArray<FDSLeaderboardItem>& LeaderboardItems);
	
};
