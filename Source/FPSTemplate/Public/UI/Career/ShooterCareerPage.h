// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "UI/portal/Dashboard/CareerPage.h"
#include "ShooterCareerPage.generated.h"

class USpecialElimData;
struct FDSRetrieveMatchStatsResponse;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UShooterCareerPage : public UCareerPage
{
	GENERATED_BODY()

public:
	virtual void OnRetrieveMatchStatsReceived(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse) override;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpecialElimData> SpecialElimData;
	
};
