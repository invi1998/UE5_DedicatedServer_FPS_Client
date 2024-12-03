// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FleetID.generated.h"

class UTextBlock;
/**
 * 舰队ID
 */
UCLASS()
class DEDICATEDSERVERS_API UFleetID : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_FleetID;
};
