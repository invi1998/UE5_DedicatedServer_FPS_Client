// // Copyright invi1998


#include "UI/portal/Dropdown/Dropdown.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UDropdown::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Expander->OnClicked.AddDynamic(this, &UDropdown::ToggleDropdown);
	Button_Expander->OnHovered.AddDynamic(this, &UDropdown::Hover);
	Button_Expander->OnUnhovered.AddDynamic(this, &UDropdown::UnHover);
}

void UDropdown::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetStyleTransparent();
	Collapse();
	UnHover();
}

void UDropdown::SetStyleTransparent()
{
	FButtonStyle ButtonStyle;
	FSlateBrush NormalBrush = TriangleDown_Brush;

	NormalBrush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	ButtonStyle.Disabled = NormalBrush;
	ButtonStyle.Normal = NormalBrush;
	ButtonStyle.Hovered = NormalBrush;
	ButtonStyle.Pressed = NormalBrush;
	
	Button_Expander->SetStyle(ButtonStyle);
}


void UDropdown::ToggleDropdown()
{
	if (bIsExpanded)
	{
		Collapse();
	}
	else
	{
		Expand();
	}
}

void UDropdown::Expand()
{
	WidgetSwitcher->SetActiveWidget(ExpandedWidget);
	bIsExpanded = true;
	Image_Triangle->SetBrush(TriangleUp_Brush);
}

void UDropdown::Collapse()
{
	WidgetSwitcher->SetActiveWidget(CollapsedWidget);
	bIsExpanded = false;
	Image_Triangle->SetBrush(TriangleDown_Brush);
}

void UDropdown::Hover()
{
	TextBlock_ButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UDropdown::UnHover()
{
	TextBlock_ButtonText->SetColorAndOpacity(NormalTextColor);
}
