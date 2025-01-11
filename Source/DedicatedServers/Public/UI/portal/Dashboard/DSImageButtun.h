// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "DSImageButtun.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSImageButtun : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Background;

	// 纹理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSImageButtun")
	TObjectPtr<UTexture2D> Texture_Background;

	// 材质实例类型
	UPROPERTY(EditDefaultsOnly, Category = "DSImageButtun")
	TObjectPtr<UMaterialInterface> ParentMaterialInstance;	// 材质实例类型

	UPROPERTY(BlueprintReadOnly, Category = "DSImageButtun")
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;	// 动态材质实例

	UPROPERTY(EditDefaultsOnly, Category = "DSImageButtun")
	float BackgroundSaturationNormal = 0.0f;	// 饱和度

	UPROPERTY(EditDefaultsOnly, Category = "DSImageButtun")
	float BackgroundSaturationHovered = 1.2f;	// 饱和度

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Title;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CNTitle;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Description;

	void SetTitle(const FString& Title);
	void SetCNTitle(const FString& Title);
	void SetDescription(const FString& Description);
	void UpdateTexture(UTexture2D* Background);

	UFUNCTION(BlueprintCallable)
	void SetBackgroundSaturation(float Saturation);	// 饱和度

protected:
	virtual void NativeConstruct() override;
	
	
};


