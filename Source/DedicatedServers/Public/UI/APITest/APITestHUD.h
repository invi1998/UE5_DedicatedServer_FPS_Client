// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "APITestHUD.generated.h"

class UAPITestOverlay;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API AAPITestHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UAPITestOverlay> APITestOverlayClass;

	void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UAPITestOverlay> APITestOverlay;
};
