// // Copyright invi1998


#include "UI/portal/SignIn/SignInOverlay.h"

#include "UI/portal/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(PortalManagerClass, TEXT("PortalManagerClass is nullptr"));
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
}
