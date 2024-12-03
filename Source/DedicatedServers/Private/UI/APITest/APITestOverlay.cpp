// // Copyright invi1998


#include "UI/APITest/APITestOverlay.h"

#include "Components/Button.h"
#include "UI/API/ListFleets/ListFleetsBox.h"
#include "UI/APITest/APITestManger.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(APITestManagerClass, TEXT("APITestManagerClass is nullptr"));
	APITestManager = NewObject<UAPITestManger>(this, APITestManagerClass);

	checkf(ListFleetsBox, TEXT("ListFleetsBox is nullptr"));
	checkf(ListFleetsBox->Button_ListFleets, TEXT("ListFleetsBox->Button_ListFleets is nullptr"));

	// 绑定舰队列表按钮点击事件
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(APITestManager, &UAPITestManger::ListFleetsButtonClicked);
}
