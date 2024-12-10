// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PortalHUD.generated.h"

class UDashboardOverlay;
class USignInOverlay;
/**
 * 门户HUD
 */
UCLASS()
class DEDICATEDSERVERS_API APortalHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USignInOverlay> SignOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USignInOverlay> SignInOverlay;

	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay;
};
