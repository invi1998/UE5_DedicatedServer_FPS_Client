// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestRecordMatchStatsOverlay.generated.h"

class UGameStatsManager;
class UButton;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestRecordMatchStatsOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_RecordMatchStats;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;

	UFUNCTION()
	void RecordMatchStatsButtonClicked();
};
