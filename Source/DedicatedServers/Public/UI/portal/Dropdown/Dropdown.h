// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dropdown.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDropdown : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUserWidget> ExpandedWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUserWidget> CollapsedWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Expander;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown")
	FSlateColor HoveredTextColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown")
	FSlateColor NormalTextColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown")
	FSlateBrush TriangleUp_Brush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown")
	FSlateBrush TriangleDown_Brush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown")
	TObjectPtr<UImage> Image_Triangle;

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void SetStyleTransparent();

private:
	UFUNCTION()
	void ToggleDropdown();

	bool bIsExpanded = false;

	UFUNCTION()
	void Expand();

	UFUNCTION()
	void Collapse();

	UFUNCTION()
	void Hover();

	UFUNCTION()
	void UnHover();

};
