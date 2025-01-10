// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSImageButtun.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSImageButtun : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget), Category = "DSImageButtun")
	TObjectPtr<UImage> Image_Background;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget), Category = "DSImageButtun")
	TObjectPtr<UImage> Image_Foreground;

	// 纹理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSImageButtun")
	TObjectPtr<UTexture2D> Texture_Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSImageButtun")
	TObjectPtr<UTexture2D> Texture_Foreground;

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
	TObjectPtr<UTextBlock> TextBlock_Description;

	void SetTitle(const FString& Title);
	void SetDescription(const FString& Description);

	UFUNCTION(BlueprintCallable)
	void SetBackgroundSaturation(float Saturation);	// 饱和度

protected:
	virtual void NativeConstruct() override;
	
};
