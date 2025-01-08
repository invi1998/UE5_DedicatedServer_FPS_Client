// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DS_MatchPlayerState.generated.h"

class UGameStatsManager;
struct FDSRecordMatchStatsInput;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnMatchTimerFinished(const FString& UserName);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

protected:
	virtual void BeginPlay() override;

	void RecordMatchStats(const FDSRecordMatchStatsInput& MatchStatsInput) const;

private:
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;

	
};
