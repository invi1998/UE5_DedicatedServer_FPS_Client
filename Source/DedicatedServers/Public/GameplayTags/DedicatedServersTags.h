#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{
		// 定义标签，用于列举服务器舰队列表（ListFleets 等价于 "DedicatedServersTags.GameSessionsAPI.ListFleets"）
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ListFleets);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FindOrCreateGameSession);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(CreatePlayerSession);
	}

	namespace PortalAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignIn);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignUp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConfirmAccount);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignOut);
	}
}

