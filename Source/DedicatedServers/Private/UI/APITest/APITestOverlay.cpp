// // Copyright invi1998


#include "UI/APITest/APITestOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "UI/API/ListFleets/FleetID.h"
#include "UI/API/ListFleets/ListFleetsBox.h"
#include "UI/APITest/APITestManger.h"
#include "HTTP/HTTPRequestTypes.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	checkf(APITestManagerClass, TEXT("APITestManagerClass is nullptr"));
	APITestManager = NewObject<UAPITestManger>(this, APITestManagerClass);

	checkf(ListFleetsBox, TEXT("ListFleetsBox is nullptr"));
	checkf(ListFleetsBox->Button_ListFleets, TEXT("ListFleetsBox->Button_ListFleets is nullptr"));

	// 绑定舰队列表按钮点击事件
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(this, &UAPITestOverlay::ListFleetsButtonClicked);
}

void UAPITestOverlay::ListFleetsButtonClicked()
{
	checkf(APITestManagerClass, TEXT("APITestManagerClass is nullptr"));

	// 绑定舰队列表响应事件
	APITestManager->OnListFleetsResponseReceived.AddDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);

	// 调用ListFleets方法
	APITestManager->ListFleets();

	// 禁用舰队列表按钮，避免重复点击
	ListFleetsBox->Button_ListFleets->SetIsEnabled(false);
}

void UAPITestOverlay::OnListFleetsResponseReceived(const FDSListFleetsResponse& ListFleetsResponse, bool bWasSuccessful)
{
	if (APITestManager->OnListFleetsResponseReceived.IsAlreadyBound(this, &UAPITestOverlay::OnListFleetsResponseReceived))
	{
		APITestManager->OnListFleetsResponseReceived.RemoveDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);
	}
	
	ListFleetsBox->ScrollBox_ListFleets->ClearChildren();
	if (bWasSuccessful)
	{
		// 显示舰队列表
		for (const FString& FleetId : ListFleetsResponse.FleetIds)
		{
			UFleetID* FleetIDWidget = CreateWidget<UFleetID>(this, ListFleetsBox->FleetIDWidgetClass);
			FleetIDWidget->SetFleetID(FleetId);
			ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIDWidget);
		}
	}
	else
	{
		// 显示错误信息
		UFleetID* FleetIDWidget = CreateWidget<UFleetID>(this, ListFleetsBox->FleetIDWidgetClass);
		FleetIDWidget->SetFleetID("Failed to list fleets");
		ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIDWidget);
	}

	ListFleetsBox->Button_ListFleets->SetIsEnabled(true);
}
