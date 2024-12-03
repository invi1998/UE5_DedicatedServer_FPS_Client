// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UAPITestManger;
class UListFleetsBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPITestManger> APITestManagerClass;

protected:
	virtual void NativeConstruct() override;		// 在构造函数中绑定事件

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListFleetsBox> ListFleetsBox;
	
	UPROPERTY()
	TObjectPtr<UAPITestManger> APITestManager;
};
