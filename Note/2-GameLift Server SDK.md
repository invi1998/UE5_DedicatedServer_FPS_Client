# GameLift Server SDK and UE5 Plugin

[将 Amazon GameLift 集成到虚幻引擎项目中 - Amazon GameLift](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-engines-setup-unreal.html)

**前置准备**

1：首先，我们需要SSL服务，它里面有两个二进制文件是我们需要的。

`libcrypto-3-x64.dll`和 `libssl-3-x64.dll`

>a: Perl - 一种编程语音
>
>b: NASM - 汇编和反汇编程序
>
>c：安装OpenSSL

2：AWS Server SDK 

3：虚幻引擎服务器SDK插件

---

###### 为虚幻引擎构建 C++ 服务器SDK库

1. 下载[适用于虚幻引擎的 Amazon GameLift C++ 服务器SDK插件](https://aws.amazon.com/gamelift/getting-started/)。

   

   ###### 注意

   由于路径超过 260 个字符的限制，将放在默认下载目录中可能会导致构建失败。SDK例如：`C:\Users\Administrator\Downloads\GameLift-SDK-Release-06_15_2023\GameLift-Cpp-ServerSDK-5.0.4`

   例如，我们建议您将移动SDK到另一个目录`C:\GameLift-Cpp-ServerSDK-5.0.4`。

2. 下载并安装 Open SSL。有关下载 Open 的更多信息SSL，请阅读 Github [Open SSL 构建和安装](https://github.com/openssl/openssl#build-and-install)文档。

   有关更多信息，请阅读适用[于 Windows 平台的 “打开SSL笔记](https://github.com/openssl/openssl/blob/master/NOTES-WINDOWS.md)” 文档。

   

   ###### 注意

   你用来构建 Amazon GameLift 服务器SSL的 Open 版本SDK应与虚幻SSL用来打包游戏服务器的 Open 版本相匹配。你可以在虚幻安装目录中找到版本信息`...Engine\Source\ThirdParty\OpenSSL`。

3. 下载库后，为虚幻引擎构建 C++ 服务器SDK库。

   在下载的`GameLift-Cpp-ServerSDK-<version>`目录中SDK，使用`-DBUILD_FOR_UNREAL=1`参数进行编译并构建服务器SDK。以下示例演示了如何使用 `cmake` 命令。

   在计算机终端中运行以下命令。

   ```shell
   mkdir cmake-build
   cmake.exe -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -S . -B ./cmake-build -DBUILD_FOR_UNREAL=1 -A x64
   cmake.exe --build ./cmake-build --target ALL_BUILD --config Release
   ```

   > 如果上述过程出错，可以查看一下环境变量是否添加了openssl的地址，没有的话添加这两个环境变量，在试一下
   >
   > ![image-20241130160700849](.\image-20241130160700849.png)

   Windows 版本将在该`out\gamelift-server-sdk\Release`文件夹中创建以下二进制文件：

   - `cmake-build\prefix\bin\aws-cpp-sdk-gamelift-server.dll`
   - `cmake-build\prefix\bin\aws-cpp-sdk-gamelift-server.lib`

   将这两个库文件复制到亚马逊 GameLift 虚幻引擎插件包中的`ThirdParty\GameLiftServerSDK\Win64`文件夹。

使用以下步骤将 Amazon GameLift 插件导入您的示例项目。

###### 导入 Amazon GameLift 插件

1. 找到您在前面的过程中从插件中提取`GameLiftServerSDK`的文件夹。

2. `Plugins`在您的游戏项目根文件夹中找到。（如果该文件夹不存在，则在那里创建。）

3. 将该`GameLiftServerSDK`文件夹复制到`Plugins`。

   这将允许虚幻项目看到该插件。

4. 将 Amazon GameLift 服务器SDK插件添加到游戏`.uproject`文件中。

   在示例中，应用程序被调用`GameLiftUnrealApp`，因此文件将被调用`GameLiftUnrealApp.uproject`。

5. 编辑`.uproject`文件以将插件添加到您的游戏项目中。

   ```json
   "Plugins": [
       {
           "Name": "GameLiftServerSDK",
           "Enabled": true
       }
   ]
   ```

6. 确保游戏依赖 ModuleRules 于插件。打开`.Build.cs`文件并添加 Amazon GameLiftServer SDK 依赖项。此文件位于 `Your-application-name/Source//Your-application-name/`。

   例如，教程的文件路径是`../GameLiftUnrealApp/Source/GameLiftUnrealApp/GameLiftUnrealApp.Build.cs`。

7. `"GameLiftServerSDK"`添加到列表的末尾`PublicDependencyModuleNames`.

   ```c#
   using UnrealBuildTool;
   using System.Collections.Generic;
   public class GameLiftUnrealApp : ModuleRules  
     {
       public GameLiftUnrealApp(TargetInfo Target)
       {
         PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameLiftServerSDK" });
         bEnableExceptions = true;
       }
     }
   ```

该插件现在应该可以用于您的应用程序。继续下一节，将Amazon GameLift 功能集成到您的游戏中。



## 将 Amazon GameLift 服务器代码添加到你的虚幻项目中

您已经配置并设置了虚幻引擎环境，现在可以将游戏服务器与Amazon集成 GameLift。本主题中介绍的代码对 Amazon GameLift 服务进行必需的调用。它还实现了一组回调函数，用于响应 Amazon GameLift 服务的请求。有关每个函数以及代码作用的更多信息，请参阅[初始化服务器进程](https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-sdk-server-api.html#gamelift-sdk-server-initialize)。有关此代码中使用的SDK操作和数据类型的更多信息，请参阅。[适用于虚幻引擎 GameLift 的 Amazon 服务器 SDK 5.x：动作](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-unreal-actions.html)

要使用 Amazon 初始化游戏服务器 GameLift，请按以下步骤操作。



###### 注意

下一节中提供的 GameLift Amazon 特定代码取决于`WITH_GAMELIFT`预处理器标志的使用。仅当满足以下两个条件时，此标志才为真：

- `Target.Type == TargetRules.TargetType.Server`
- 这些插件找到了 Amazon GameLift 服务器的SDK二进制文件。

这样可以确保只有虚幻服务器版本才能调用Amazon GameLift 的后端API。它还允许您编写能够针对您的游戏可能产生的所有不同虚幻目标正确执行的代码。

###### 将游戏服务器与 Amazon 集成 GameLift

1. 在 Visual Studio 中，打开您的应用程序的`.sln`文件。在我们的示例中，该文件`GameLiftUnrealApp.sln`位于根文件夹中。

2. 打开解决方案后，找到应用程序的`Your-application-nameGameMode.h`文件。示例：`GameLiftUnrealAppGameMode.h`。

3. 更改头文件以使其与以下示例代码保持一致。请务必用您自己的应用程序名称替换 GameLiftUnrealApp “”。

   ```cpp
   #pragma once
   
   #include "CoreMinimal.h"
   #include "GameFramework/GameModeBase.h"
   #include "GameLiftServerSDK.h"
   #include "GameLiftUnrealAppGameMode.generated.h"
   
   DECLARE_LOG_CATEGORY_EXTERN(GameServerLog, Log, All);
   
   UCLASS(minimalapi)
   class AGameLiftUnrealAppGameMode : public AGameModeBase
   {
       GENERATED_BODY()
   
   public:
       AGameLiftUnrealAppGameMode();
   
   protected:
       virtual void BeginPlay() override;
   
   private:
       // Process Parameters needs to remain in scope for the lifetime of the app
       FProcessParameters m_params;
   
       void InitGameLift();
   };
   ```

4. 打开相关的源文件`Your-application-nameGameMode.cpp`文件。在我们的示例:`GameLiftUnrealAppGameMode.cpp`. 中，更改代码以与以下示例代码保持一致。请务必用您自己的应用程序名称替换 GameLiftUnrealApp “”。

   此示例演示如何添加与亚马逊集成所需的所有元素 GameLift，如[将亚马逊 GameLift添加到您的游戏服务器](https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-sdk-server-api.html)中所述。这包括：

   - 初始化 Amazon GameLift API 客户端。
   - 实现回调函数以响应 Amazon GameLift 服务的请求`OnStartGameSession`，包括`OnProcessTerminate`、和`onHealthCheck`。
   - 使用指定端口调用 ProcessReady ()，以便在准备举办游戏会话 GameLiftservice 时通知 Amazon。

   ```c++
   #include "GameLiftUnrealAppGameMode.h"
   #include "GameLiftUnrealAppCharacter.h"
   
   #include "UObject/ConstructorHelpers.h"
   
   DEFINE_LOG_CATEGORY(GameServerLog);
   
   AGameLiftUnrealAppGameMode::AGameLiftUnrealAppGameMode()
   {
       // set default pawn class to our Blueprinted character
       static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
       if (PlayerPawnBPClass.Class != NULL)
       {
           DefaultPawnClass = PlayerPawnBPClass.Class;
       }
   }
   
   void AGameLiftUnrealAppGameMode::BeginPlay()
   {
   #if WITH_GAMELIFT
       InitGameLift();
   #endif
   }
   
   void AGameLiftUnrealAppGameMode::InitGameLift()
   {
       UE_LOG(GameServerLog, Log, TEXT("Initializing the GameLift Server"));
   
       //Getting the module first.
       FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
   
       //Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
       FServerParameters serverParameters;
   
       //AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
       if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), serverParameters.m_authToken))
       {
           UE_LOG(GameServerLog, Log, TEXT("AUTH_TOKEN: %s"), *serverParameters.m_authToken)
       }
   
       //The Host/compute-name of the GameLift Anywhere instance.
       if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), serverParameters.m_hostId))
       {
           UE_LOG(GameServerLog, Log, TEXT("HOST_ID: %s"), *serverParameters.m_hostId)
       }
   
       //The Anywhere Fleet ID.
       if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), serverParameters.m_fleetId))
       {
           UE_LOG(GameServerLog, Log, TEXT("FLEET_ID: %s"), *serverParameters.m_fleetId)
       }
   
       //The WebSocket URL (GameLiftServiceSdkEndpoint).
       if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), serverParameters.m_webSocketUrl))
       {
           UE_LOG(GameServerLog, Log, TEXT("WEBSOCKET_URL: %s"), *serverParameters.m_webSocketUrl)
       }
   
       //The PID of the running process
       serverParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
       UE_LOG(GameServerLog, Log, TEXT("PID: %s"), *serverParameters.m_processId);
   
       //InitSDK establishes a local connection with GameLift's agent to enable further communication.
       //Use InitSDK(serverParameters) for a GameLift Anywhere fleet. 
       //Use InitSDK() for a GameLift managed EC2 fleet.
       gameLiftSdkModule->InitSDK(serverParameters);
   
       //Implement callback function onStartGameSession
       //GameLift sends a game session activation request to the game server
       //and passes a game session object with game properties and other settings.
       //Here is where a game server takes action based on the game session object.
       //When the game server is ready to receive incoming player connections, 
       //it invokes the server SDK call ActivateGameSession().
       auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
       {
           FString gameSessionId = FString(gameSession.GetGameSessionId());
           UE_LOG(GameServerLog, Log, TEXT("GameSession Initializing: %s"), *gameSessionId);
           gameLiftSdkModule->ActivateGameSession();
       };
   
       m_params.OnStartGameSession.BindLambda(onGameSession);
   
       //Implement callback function OnProcessTerminate
       //GameLift invokes this callback before shutting down the instance hosting this game server.
       //It gives the game server a chance to save its state, communicate with services, etc., 
       //and initiate shut down. When the game server is ready to shut down, it invokes the 
       //server SDK call ProcessEnding() to tell GameLift it is shutting down.
       auto onProcessTerminate = [=]() 
       {
           UE_LOG(GameServerLog, Log, TEXT("Game Server Process is terminating"));
           gameLiftSdkModule->ProcessEnding();
       };
   
       m_params.OnTerminate.BindLambda(onProcessTerminate);
   
       //Implement callback function OnHealthCheck
       //GameLift invokes this callback approximately every 60 seconds.
       //A game server might want to check the health of dependencies, etc.
       //Then it returns health status true if healthy, false otherwise.
       //The game server must respond within 60 seconds, or GameLift records 'false'.
       //In this example, the game server always reports healthy.
       auto onHealthCheck = []() 
       {
           UE_LOG(GameServerLog, Log, TEXT("Performing Health Check"));
           return true;
       };
   
       m_params.OnHealthCheck.BindLambda(onHealthCheck);
   
       //The game server gets ready to report that it is ready to host game sessions
       //and that it will listen on port 7777 for incoming player connections.
       m_params.port = 7777;
   
       //Here, the game server tells GameLift where to find game session log files.
       //At the end of a game session, GameLift uploads everything in the specified 
       //location and stores it in the cloud for access later.
       TArray<FString> logfiles;
       logfiles.Add(TEXT("GameLift426Test/Saved/Logs/GameLift426Test.log"));
       m_params.logParameters = logfiles;
   
       //The game server calls ProcessReady() to tell GameLift it's ready to host game sessions.
       UE_LOG(GameServerLog, Log, TEXT("Calling Process Ready"));
       gameLiftSdkModule->ProcessReady(m_params);
   }
   ```

5. 为以下两种目标类型构建游戏项目：*开发编辑器和**开发服务器*。

   

   ###### 注意

   您不需要重新构建解决方案。相反，只在与您的应用程序名称相匹配的`Games`文件夹下构建项目。否则，Visual Studio 会重建整个UE5项目，这可能需要长达一个小时的时间。

6. 两个构建都完成后，关闭Visual Studio并打开项目`.uproject`文件以在虚幻编辑器中将其打开。

7. 在虚幻编辑器中，打包游戏的服务器版本。要选择目标，请前往 “**平台**”、“**窗口**”，然后选择 **`Your-application-nameServer`**.

8. 要开始构建服务器应用程序的过程，请转到**平台**、**Windows**，然后选择**Package Project**。构建完成后，您应该有一个可执行文件。在我们的示例中，文件名为`GameLiftUnrealAppServer.exe`。

9. 在虚幻编辑器中构建服务器应用程序会生成两个可执行文件。一个位于游戏构建文件夹的根目录中，用作实际服务器可执行文件的封装器。

   使用您的服务器版本创建 Amazon GameLift 队列时，我们建议您传入实际的服务器可执行文件作为运行时配置启动路径。例如，在您的游戏版本文件夹中，您的根目录可能有一个`GameLiftFPS.exe`文件，另一个位于根目录`\GameLiftFPS\Binaries\Win64\GameLiftFPSServer.exe`。创建队列时，我们建议您使用`C:\GameLiftFPS\Binaries\Win64\GameLiftFPSServer.exe`作为运行时配置的启动路径。

10. 确保在 Amazon GameLift 舰队上打开必要的UDP端口，以便游戏服务器可以与游戏客户端通信。默认情况下， Unreal Engine 使用端口`7777`。有关更多信息，请参阅 [UpdateFleetPortSettings](https://docs.aws.amazon.com/gamelift/latest/apireference/API_UpdateFleetPortSettings.html)Amazon GameLift 服务API参考指南。

11. 为您的游戏版本创建`install.bat`文件。每当游戏版本部署到 Amazon GameLift 舰队时，都会运行此安装脚本。下面给出了一个示例文件：

    ```
    VC_redist.x64.exe /q
    UE5PrereqSetup_x64.exe /q
    ```

    对于某些版本的虚幻引擎，`install.bat`应该改为

    ```
    VC_redist.x64.exe /q
    UEPrereqSetup_x64.exe /q
    ```

    

    ###### 注意

    该文件的文件路径是`Engine\Extras\Redist\en-us`。`<>PrereqSetup_x64.exe`

12. 现在，您可以打包游戏版本并将其上传到 Amazon GameLift。

    包含游戏版本的 O SSL pen you package 版本需要与游戏引擎在构建游戏服务器时使用的版本相匹配。确保在游戏服务器SSL版本中打包正确的开放版本。对于 Windows 操作系统，“打开” SSL 格式为`.dll`。

    

    ###### 注意

    将 Open 打包到你的游戏服务器版本SSLDLLs中。请务必打包与您在构建游戏服务器时使用的相同版本的 Open SSL。

    - `libssl-1_1-x64.dll`

      `libcrypto-1_1-x64.dll`

    Package 将依赖项和游戏服务器可执行文件一起打包到 zip 文件的根目录中。例如，`openssl-lib`dll 应与`.exe`文件位于同一目录中。

## 后续步骤

你已经配置并设置了虚幻引擎环境，现在可以开始将Amazon GameLift 集成到你的游戏中了。