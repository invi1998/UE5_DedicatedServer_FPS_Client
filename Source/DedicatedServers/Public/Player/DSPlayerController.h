// // Copyright invi1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADSPlayerController();

	virtual void ReceivedPlayer() override;	// 玩家控制器初始化

protected:

	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeStamp);

	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeStamp);
	
private:
	float SingleTripTime = 0.0f;	// 单程时间
};
