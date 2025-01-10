// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CareerPage.generated.h"

class UTextBlock;
class UDSImageButtun;
class UScrollBox;
struct FDSRetrieveMatchStatsResponse;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UCareerPage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UserName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Status;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Achievements;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDSImageButtun> CareerAchievementButtonClass;

	UFUNCTION()
	void OnRetrieveMatchStatsReceived(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse);

	UFUNCTION()
	void SetMatchStats(const FString& StatusStr, bool bNeedResetButton);

protected:
	virtual void NativeConstruct() override;
	
};
