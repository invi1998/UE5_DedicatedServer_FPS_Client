// // Copyright invi1998


#include "Lobby/LobbyPlayerInfo.h"

void FLobbyPlayerInfoArray::AddPlayer(const FLobbyPlayerInfo& Player)
{
	// 我们不仅需要添加玩家信息，还需要获取添加的玩家信息的索引，用于脏标记
	const int32 Index = Players.Add(Player);
	// 添加玩家信息后，我们需要将该玩家信息的索引添加到脏标记数组中
	MarkItemDirty(Players[Index]);
	// 然后调用PostReplicatedAdd，即添加元素后的处理回调
	Players[Index].PostReplicatedAdd(*this);
}

void FLobbyPlayerInfoArray::RemovePlayer(const FString& PlayerSessionId)
{
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		if (Players[i].PlayerSessionId == PlayerSessionId)
		{
			// PreReplicatedRemove，在删除元素之前执行的操作
			Players[i].PreReplicatedRemove(*this);
			
			// 移除玩家信息, RemoveAtSwap是一个高效的删除操作, 会将最后一个元素移动到删除的位置, 然后删除最后一个元素
			// 而不用像RemoveAt那样，将删除的元素后面的元素向前移动
			Players.RemoveAtSwap(i);

			// 移除玩家信息后，我们需要将该玩家信息的索引添加到脏标记数组中
			MarkArrayDirty();
			
			break;
		}
	}
}
