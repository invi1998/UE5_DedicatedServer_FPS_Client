// // Copyright invi1998


#include "UI/portal/Dashboard/GameLoading.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/portal/Dashboard/InfoText.h"

void UGameLoading::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(VerticalBox_LoadingTextBox))
	{
		VerticalBox_LoadingTextBox->ClearChildren();
	}
}

void UGameLoading::AddLoadingText(const FString& Text)
{
	if (IsValid(VerticalBox_LoadingTextBox))
	{
		UInfoText *InfoText = CreateWidget<UInfoText>(this, InfoTextClass);
		if (IsValid(InfoText))
		{
			InfoText->TextBlock_Info->SetText(FText::FromString(Text));
			VerticalBox_LoadingTextBox->AddChild(InfoText);
		}
	}
}
