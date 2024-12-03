// // Copyright invi1998


#include "Data/API/APIData.h"

FString UAPIData::GetAPIEndPoint(const FGameplayTag& Resource) const
{
	FString ResourceName = Resources.FindChecked(Resource);
	return InvokeURL + "/" + Stage + "/" + ResourceName;
}
