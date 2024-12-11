﻿// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PortalManagement.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPortalManagement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEDICATEDSERVERS_API IPortalManagement
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RefreshToken(const FString& RefreshToken) = 0;

};
