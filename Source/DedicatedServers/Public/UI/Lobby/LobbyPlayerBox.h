// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerBox.generated.h"

class UPlayerLabel;
class UScrollBox;
class ALobbyState;
struct FLobbyPlayerInfo;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULobbyPlayerBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayerList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerLabel> PlayerLabelClass;

protected:
	virtual void NativeOnInitialized() override;

	void UpdatePlayerInfo(ALobbyState* InLobbyState);

	UFUNCTION()
	void AddPlayer(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION()
	void RemovePlayer(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION()
	void LobbyStateInitialized(ALobbyState* InLobbyState);

private:
	UPlayerLabel* FindPlayerLabel(const FString& PlayerId) const;
	
};

