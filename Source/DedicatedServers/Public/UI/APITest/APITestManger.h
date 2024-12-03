// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "APITestManger.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestManger : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ListFleetsButtonClicked();
	
private:
	
};
