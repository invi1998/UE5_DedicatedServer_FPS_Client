// // Copyright invi1998


#include "UI/API/ListFleets/FleetID.h"

#include "Components/TextBlock.h"

void UFleetID::SetFleetID(const FString& inFleetsId) const
{
	check(TextBlock_FleetID);
	TextBlock_FleetID->SetText(FText::FromString(inFleetsId));
}
