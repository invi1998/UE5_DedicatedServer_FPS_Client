// // Copyright invi1998


#include "UI/portal/Dashboard/DSImageButtun.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDSImageButtun::SetTitle(const FString& Title)
{
	TextBlock_Title->SetText(FText::FromString(Title));
}

void UDSImageButtun::SetDescription(const FString& Description)
{
	TextBlock_Description->SetText(FText::FromString(Description));
}

void UDSImageButtun::SetBackgroundSaturation(float Saturation)
{
	if (IsValid(DynamicMaterialInstance))
	{
		DynamicMaterialInstance->SetScalarParameterValue(FName("Saturation"), Saturation);
	}
}

void UDSImageButtun::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(ParentMaterialInstance)) return;
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterialInstance, this);
	if (IsValid(DynamicMaterialInstance))
	{
		// 设置纹理
		DynamicMaterialInstance->SetTextureParameterValue(FName("Texture"), Texture_Background);
		DynamicMaterialInstance->SetScalarParameterValue(FName("Saturation"), BackgroundSaturationNormal);
		
		Image_Background->SetBrushFromMaterial(DynamicMaterialInstance);
	}
}
