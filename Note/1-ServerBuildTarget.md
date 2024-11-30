# ServerBuildTarget

在Unreal Engine 5 (UE5)中，TargetRules是用来定义编译目标（target）的行为和属性的一组规则。当你创建一个新项目或者添加一个新的游戏模式、编辑器插件或工具时，UE5会为你生成一个C++类，这个类继承自`ModuleRules`并覆盖了`GetTargetRules`函数，该函数返回一个`TargetTypeRules`对象，其中包含了编译该模块所需的所有信息。

TargetRules通常包括以下内容：

1. **平台特定设置**：你可以为不同的平台（如Windows, MacOS, iOS, Android等）指定不同的编译选项和行为。
2. **构建配置**：可以为Debug、Development、Shipping等不同的构建类型指定配置。
3. **依赖关系**：你能够指定你的模块依赖哪些其他的模块，确保它们按照正确的顺序被加载和链接。
4. **第三方库集成**：如果你的项目需要使用第三方库，你可以在TargetRules中指定这些库的位置和如何链接它们。
5. **额外的编译标志和预处理器宏**：可以添加额外的编译器标志或定义预处理器宏来改变代码的行为。
6. **资源处理**：可以控制哪些资源应该被打包进最终产品，以及如何处理这些资源。
7. **其他自定义设置**：根据项目的特殊需求，可能还需要一些额外的定制化设置。

TargetRules是在`Build.cs`文件中定义的，每个包含C++代码的模块都有一个对应的`Build.cs`文件。这个文件是用C#编写的，并且由Unreal Build Tool (UBT)解析以决定如何编译项目。

对于我们当前的项目，我们有Editor和Game两种编译规则，现在，对于专用服务器，我们需要一个特定的Target.cs，用于定义我们的服务器编译行为。

如下，我们在项目里新增了一个FPSTemplateServer.Target.cs文件，然后将其目标类型设置为Server.

```csharp
// Copyright invi1998

using UnrealBuildTool;
using System.Collections.Generic;

public class FPSTemplateServerTarget : TargetRules
{
    public FPSTemplateServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("FPSTemplate");
    }
}

```

有了这个编译规则后，现在在我们的Vs里，编译列表选项里，应该就有Server选项用于服务器构建

![image-20241130131908528](.\image-20241130131908528.png)

当然，如果直接构建项目，此时可能会有报错或者其他怪异行为，建议添加好cs后，重新生成VS项目

清理临时文件后，重新生成VS项目，然后选择项目模式为Server，然后构建项目，自此，服务器开发的前期准备就算完成