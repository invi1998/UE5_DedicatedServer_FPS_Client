#include "GameplayTags/DedicatedServersTags.h"

namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{
		// 定义标签，用于列举服务器舰队列表（ListFleets 等价于 "DedicatedServersTags.GameSessionsAPI.ListFleets"）
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, "DedicatedServersTags.GameSessionsAPI.ListFleets", "列举服务器舰队列表");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, "DedicatedServersTags.GameSessionsAPI.FindOrCreateGameSession", "查找或创建游戏会话：查找活跃的可用游戏会话，如果没有找到，则创建一个新的游戏会话");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession, "DedicatedServersTags.GameSessionsAPI.CreatePlayerSession", "创建玩家会话：为玩家创建一个新的会话，或将玩家添加到现有会话中");
		
	}

	namespace PortalAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignIn, "DedicatedServersTags.PortalAPI.SignIn", "登录");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignUp, "DedicatedServersTags.PortalAPI.SignUp", "注册");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConfirmAccount, "DedicatedServersTags.PortalAPI.ConfirmAccount", "确认账户");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignOut, "DedicatedServersTags.PortalAPI.SignOut", "登出");
	}

	namespace GameStatsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RecordMatchStats, "DedicatedServersTags.GameStatsAPI.RecordMatchStats", "记录比赛统计数据");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RetrieveMatchStats, "DedicatedServersTags.GameStatsAPI.RetrieveMatchStats", "检索比赛统计数据");
	}
}

