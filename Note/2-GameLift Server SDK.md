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

   ```
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

   ```
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

   ```
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