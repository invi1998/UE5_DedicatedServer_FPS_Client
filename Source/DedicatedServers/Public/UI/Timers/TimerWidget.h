// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/DSTypes.h"
#include "TimerWidget.generated.h"

class UCommonTextBlock;
class ADSPlayerController;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECountdownTimerType TimerType = ECountdownTimerType::NONE;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADSPlayerController> OwningPlayerController;

	UPROPERTY(BlueprintReadOnly)
	bool bActive = false;	// 是否激活

	UPROPERTY(EditDefaultsOnly)
	bool bCanBeNegative = false;	// 是否可以为负数

	UPROPERTY(EditDefaultsOnly)
	bool bShowMilliseconds = false;	// 是否显示毫秒

	UPROPERTY(EditDefaultsOnly)
	bool bHideOnZero = false;	// 是否在0时隐藏

	UFUNCTION()
	virtual void OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType InTimerType);

	UFUNCTION()
	virtual void OnTimerFinished(float CountdownTimeLeft, ECountdownTimerType InTimerType);

	FString FormatTimeAsString(float TimeInSeconds) const;

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Timer Updated"))
	void K2_OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType InTimerType);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Timer Finished"))
	void K2_OnTimerFinished(float CountdownTimeLeft, ECountdownTimerType InTimerType);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock_Timer;

	float InternalCountdownTimeLeft = 0.0f;

	void TimerStart(float InitialTime);
	void TimerStop();
	void UpdateCountdownTime(float NewTime);
};
