# 将 Amazon GameLift 添加到您的游戏服务器

[原文链接]([将 Amazon GameLift 添加到您的游戏服务器 - Amazon GameLift](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/gamelift-sdk-server-api.html))

## 初始化服务器进程

添加代码以建立与 Amazon 的通信 GameLift 并报告服务器进程已准备好托管游戏会话。此代码必须在任何 Amazon GameLift 代码之前运行。

1. 通过调用初始化 Amazon GameLift API 客户端`InitSdk()`。要初始化在 Amazon GameLift 托管EC2实例上运行的游戏服务器进程，请使用不带参数的默认 `InitSDK()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-initsdk)) ([C++) (C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-initsdk)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-initsdk)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-initsdk)。亚马逊 GameLift 会自动 GameLift 为您连接亚马逊。 初始化在 Amazon 上运行的游戏服务器进程 GameLift Anywhere 计算资源，`InitSdk()`使用以下命令调用`ServerParameters`：

   - 用于连接到游戏服务器的 websocket 的。URL
   - 用于托管游戏服务器的进程的 ID。
   - 托管游戏服务器进程的计算的 ID。
   - 包含您的 Amazon 的 GameLift 车队的 ID GameLift Anywhere 计算。
   - Amazon GameLift 操作生成的授权令牌`GetComputeAuthToken`。

2. 通知 Amazon GameLift 服务器进程已准备好托管游戏会话。使用以下信息致电 `ProcessReady()` ([C ++) (C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-processready)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-processready)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-processready)) 。（请注意，每个服务器进程只能调用 `ProcessReady()` 一次）。

   - 服务器进程使用的端口号。后端服务向游戏客户端提供端口号和 IP 地址，用于连接到服务器进程并加入游戏会话。
   - 您希望 Amazon GameLift 保留的文件的位置，例如游戏会话日志。服务器进程在游戏会话期间生成这些文件。它们暂时存储在服务器进程正在运行的实例上，当实例关闭时，它们就会丢失。您发布的所有文件都将上传到 Amazon GameLift。您可以通过[亚马逊 GameLift 控制台或调用亚马逊](https://console.aws.amazon.com/gamelift) GameLift API操作来访问这些文件 [GetGameSessionLogUrl()](https://docs.aws.amazon.com/gamelift/latest/apireference/API_GetGameSessionLogUrl.html)。
   - Amazon GameLift 可以调用到您的服务器进程的回调函数的名称。游戏服务器代码必须执行这些函数。有关更多信息，请参阅 (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-datatypes.html#integration-server-sdk5-cpp-dataypes-process)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-datatypes.html#integration-server-sdk5-csharp-dataypes-process)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-datatypes.html#integration-server-sdk5-unreal-dataypes-process)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-datatypes.html#integration-server-sdk-go-dataypes-process)) 。
     - （可选）`onHealthCheck`— Amazon 定期 GameLift 调用此函数向服务器请求运行状况报告。
     - `onStartGameSession`— Amazon GameLift 调用此函数是为了响应客户请求 [CreateGameSession()](https://docs.aws.amazon.com/gamelift/latest/apireference/API_CreateGameSession.html)。
     - `onProcessTerminate`— Amazon GameLift 强制服务器进程停止，让它优雅地关闭。
     - （可选）`onUpdateGameSession`— Amazon GameLift 向游戏服务器提供更新的游戏会话对象，或者根据匹配回填请求提供状态更新。[FlexMatch 回填](https://docs.aws.amazon.com/gamelift/latest/flexmatchguide/match-backfill.html)功能需要此回调。

   您也可以设置游戏服务器来安全地访问您拥有或控制的 AWS 资源。有关更多信息，请参阅 [与舰队中的其他 AWS 资源进行沟通](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/gamelift-sdk-server-resources.html)。

## 报告服务器进程运行状况

向游戏服务器添加代码以实现回调函数 `onHealthCheck()`。Amazon 会定期 GameLift 调用此回调方法来收集运行状况指标。要实现此回调函数，执行以下操作：

- 评估服务器进程的运行状况。例如，如果任何外部依赖项失败，您可将服务器进程报告为不正常。
- 完成运行状况评估并在 60 秒内响应回调。如果 Amazon 在这段时间内 GameLift 没有收到响应，它会自动认为服务器进程不正常。
- 返回布尔值：true 表示正常，false 表示不正常。

如果您未实现运行状况检查回调，Amazon 会 GameLift 认为服务器进程正常，除非服务器不响应。

Amazon GameLift 使用服务器进程运行状况来结束不健康的进程并清理资源。如果服务器进程继续报告为运行状况不佳或连续三次运行状况检查没有响应，那么 Amazon GameLift 可能会关闭该进程并启动一个新的进程。Amazon GameLift 收集有关队列服务器进程运行状况的指标。

## （可选）获取TLS证书

如果服务器进程在已激活TLS证书生成功能的队列上运行，则可以检索TLS证书以与游戏客户端建立安全连接并加密客户端服务器通信。证书的副本存储在实例上。要获取文件位置，请调用 `GetComputeCertificate()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-getcomputecertificate)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-getcomputecertificate)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-getcomputecertificate)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-getcomputecertificate)) 。

## 启动游戏会话

添加代码以实现回调函数 `onStartGameSession`。Amazon GameLift 调用此回调在服务器上启动游戏会话。

该`onStartGameSession`函数将[GameSession](https://docs.aws.amazon.com/gamelift/latest/apireference/API_GameSession.html)对象作为输入参数。此对象包含关键的游戏会话信息，例如最大玩家人数。它还可以包括游戏数据和玩家数据。函数实现应完成以下任务：

- 启动操作以基于 `GameSession` 属性创建新的游戏会话。游戏服务器至少必须关联游戏会话 ID，游戏客户端在连接到服务器进程时会引用该会话 ID。
- 根据需要处理游戏数据和玩家数据。这些数据在 `GameSession` 对象中。
- GameLift 当新的游戏会话准备好接受玩家时，请通知 Amazon。调用服务器API操作 `ActivateGameSession()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-activategamesession)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-activategamesession)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-activategamesession)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-activategamesession)) 。在成功调用后，Amazon 将游戏会话状态 GameLift 更改为`ACTIVE`。

## （可选）验证新玩家

如果您正在跟踪玩家会话的状态，请添加代码以在新玩家连接到游戏服务器时对其进行验证。Amazon 会 GameLift 跟踪当前玩家和可用的游戏会话老虎机。

为了进行验证，请求访问游戏会话的游戏客户端必须包含玩家会话 ID。当玩家要求使用 [StartGameSessionPlacement() 或 ](https://docs.aws.amazon.com/gamelift/latest/apireference/API_StartGameSessionPlacement.html)[StartMatchmaking()](https://docs.aws.amazon.com/gamelift/latest/apireference/API_StartMatchmaking.html) 加入游戏时，Amazon 会 GameLift 自动生成此 ID。然后，玩家会话将在游戏会话中保留空闲位置。

当游戏服务器进程收到游戏客户端连接请求时，它会使用玩家会话 ID 调用 `AcceptPlayerSession()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-acceptplayersession)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-acceptplayersession)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-acceptplayersession)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-acceptplayersession)。作为回应，Amazon 会 GameLift 验证玩家会话 ID 是否与游戏会话中预留的空位相对应。Amazon GameLift 验证玩家会话 ID 后，服务器进程接受连接。然后，玩家可以加入游戏会话。如果 Amazon GameLift 未验证玩家会话 ID，则服务器进程会拒绝连接。

## 报告玩家会话结束

如果您正在跟踪玩家会话的状态，请添加代码以便在玩家离开游戏会话 GameLift 时通知 Amazon。只要服务器进程检测到断开的连接，此代码就应运行。Amazon GameLift 使用此通知来跟踪游戏会话中的当前玩家和可用老虎机。

要处理断开的连接，请在代码中添加对服务器API操作 `RemovePlayerSession()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-removeplayersession)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-removeplayersession)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-removeplayersession)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-removeplayersession)) (C++) (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-removeplayersession)) 的调用，并附上相应的玩家会话 ID。

## 结束游戏会话

在服务器进程关闭序列中添加代码，以便在游戏会话结束 GameLift 时通知 Amazon。为了回收和刷新托管资源，Amazon 会在游戏会话完成后 GameLift 关闭服务器进程。

在服务器进程关闭代码开始时，调用服务器API操作 `ProcessEnding()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-processending)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-processending)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-processending)) (G [o](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-processending)) g (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-processending)) 。此调用通知 Amazon GameLift 服务器进程正在关闭。Amazon 将游戏会话状态和服务器进程状态 GameLift 更改为`TERMINATED`。调用 `ProcessEnding()` 后，可以安全地关闭进程。

## 回应服务器进程关闭通知

添加代码以关闭服务器进程，以响应 Amazon 的通知 GameLift。当服务器进程持续报告运行状况不佳或服务器进程正在运行的实例被终止时，Amazon 会 GameLift 发送此通知。Amazon GameLift 可以在容量缩减事件中或响应竞价型实例中断时停止实例。

要处理关机通知，请对游戏服务器代码进行以下更改：

- 实现回调函数 `onProcessTerminate()`。此函数应调用用于关闭游戏服务器的代码。当 Amazon GameLift 调用此操作时，竞价型实例中断会在两分钟内发出通知。收到此调用后，服务器进程有五分钟的时间正常断开玩家连接、保留游戏状态数据和执行其他清除任务。
- 从游戏服务器关闭代码中调用服务器API操作 `GetTerminationTime()` ([C ++) (C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-getterm)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-getterm)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-getterm)。如果 Amazon GameLift 已发出停止服务器进程的呼叫，则`GetTerminationTime()`返回预计的终止时间。
- 在游戏服务器关闭代码开始时，调用服务器API操作 `ProcessEnding()` (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-actions.html#integration-server-sdk5-cpp-processending)) ([C#](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-csharp-actions.html#integration-server-sdk5-csharp-processending)) ([Unreal](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html#integration-server-sdk5-unreal-processending)) ([Go](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-go-actions.html#integration-server-sdk-go-processending)) (C [++](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-processending)) 。此调用 GameLift 通知亚马逊服务器进程正在关闭， GameLift然后 Amazon 将服务器进程状态更改为。`TERMINATED`调用 `ProcessEnding()` 后，可以安全地关闭进程。