#include "GameplayTags/DedicatedServersTags.h"

namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{
		// 定义标签，用于列举服务器舰队列表（ListFleets 等价于 "DedicatedServersTags.GameSessionsAPI.ListFleets"）
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, "DedicatedServersTags.GameSessionsAPI.ListFleets", "列举服务器舰队列表");

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, "DedicatedServersTags.GameSessionsAPI.FindOrCreateGameSession", "查找或创建游戏会话：查找活跃的可用游戏会话，如果没有找到，则创建一个新的游戏会话");
	}
}

