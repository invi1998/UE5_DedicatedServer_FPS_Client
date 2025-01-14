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

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ErrorColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor SuccessColor = FLinearColor::Green;

	UFUNCTION()
	void PopulateLeaderboard(const TArray<FDSLeaderboardItem>& LeaderboardItems);

	UFUNCTION()
	void SetStatusMessage(const FString& StatusMessage, bool bIsError);
	
};
