# 创建亚马逊 GameLift Anywhere 实例集

[原文链接]([创建亚马逊 GameLift Anywhere 实例集 - Amazon GameLift](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/fleets-creating-anywhere.html))

## 开始之前

在创建 Anywhere 队列之前，请执行以下任务。有关更详细的指导，请参阅[使用 Amazon GameLift Anywhere 托管的开发路线图](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/gamelift-roadmap-anywhere.html)或[使用 Amazon 进行混合托管的开发路线图 GameLift](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/gamelift-roadmap-hybrid.html)。

- **将您的游戏服务器代码与 Amazon GameLift 服务器SDK版本 5.x（或更高版本）集成。**您无需完成所有游戏集成任务，只需完成构建游戏服务器所需的任务即可。常见的做法是将本地计算机设置为 Anywhere 队列，然后使用命令行界面测试游戏服务器集成（参见[在 Amazon 上设置本地测试 GameLift Anywhere](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-testing.html)）。您可以在开发时加入其他组件（例如 GameLift 支持 Amazon 的游戏客户端）。
- **Package 将游戏服务器软件打包到你的 Anywhere 机群计算机上。**该软件包应包括您的集成游戏服务器版本以及运行游戏服务器所需的所有支持软件。
- **决定是否将 Amazon GameLift 代理与您的 Anywhere 车队配合使用。**Agent 是一种计算过程管理工具，可自动执行一些与管理服务器进程和计算相关的关键任务，以便与 Amazon 一起使用。 GameLift有关更多信息，请参阅 [与亚马逊 GameLift 代理合作](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-dev-iteration-agent.html)。

## 创建自定义位置

创建一个自定义位置来表示您的计算资源的物理位置。创建 Anywhere 队列时，必须至少定义一个自定义位置。您可以随时创建其他自定义位置并将其添加到现有车队中。

**创建自定义位置**

使用 Amazon GameLift 控制台或 AWS Command Line Interface (AWS CLI) 创建自定义位置。

- Console

  在 [Amazon GameLift 控制台](https://console.aws.amazon.com/gamelift/)中，使用导航窗格打开**位置**页面。选择 “**创建位置**” 以打开 “创建” 对话框。

  1. 在对话框中，输入**位置名称**。最佳做法是，使用描述一组计算资源的有意义位置的名称。它可能是地理位置、数据中心名称或其他位置标识符。Ama GameLift zon 会在您的自定义位置名称后面加上 cu **stom-**。
  2. （可选）向您的自定义位置添加标签。每个标签都包含定义的一个密钥和一个可选值。为要按用途、所有者或环境等有用方式分类的 AWS 资源分配标签。为每个要添加的标签选择**添加新标签**。
  3. 选择**创建**。

- AWS CLI

  使用 [`create-location`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/create-location.html) 命令创建自定义位置。提供一个`location-name`值，该值必须以开头`custom-`。最佳做法是，使用描述一组计算资源的有意义位置的名称。它可能是地理位置、数据中心名称或其他位置标识符。

  ```
  aws gamelift create-location \
      --location-name custom-location-1
  ```

  输出

  ```
  {
      "Location": {
          "LocationName": "custom-location-1",
          "LocationArn": "arn:aws:gamelift:us-east-1:111122223333:location/custom-location-1"
      }
  }
  ```



## 创建 “任何地方” 舰队

为您拥有的一组计算资源创建一个 Anywhere 队列。新的 Anywhere 队列一开始是空的；您可以通过注册计算向队列中添加计算。

创作时，一个新的 Anywhere 舰队在舰队状态下快速移动`NEW`。`ACTIVE`您可以在舰队到达`ACTIVE`队列后向其添加计算。

**创建 Anywhere 舰队**

使用 Amazon GameLift 控制台或 AWS Command Line Interface (AWS CLI) 创建 Anywhere 队列。

- Console

  在 [Amazon GameLift 控制台](https://console.aws.amazon.com/gamelift/)中，使用导航窗格打开 **Fleets** 页面。选择**创建队列**以启动队列创建工作流程。

  - **步骤 1 选择计算类型**

    选择 “**任何地方**” 选项，然后选择 “**下一步**”。

  - **第 2 步：定义舰队详情**

    在此步骤中，指定一些关键队列范围的设置。填写**舰队详情**部分：输入舰队**名称**。我们建议使用舰队命名模式，以便在查看舰队列表时更容易识别舰队类型。提供舰队的简短**描述**。根据需要设置这些可选**的其他详细信息**。您可以稍后更新这些队列设置。创建用于生产或生产前测试的队列时，请使用此设置为队列的计算指定每小时**成本**值。Amazon GameLift 可以在游戏会话放置过程中使用这些信息根据成本选择托管资源。如果要合并此队列和其他队列的指标数据，请指定**指标组**名称。对于要合并到一起的所有队列，请使用相同的指标组名称。查看指标组的指标以查看聚合数据。向您的自定义位置添加可选标签。每个标签都包含定义的一个密钥和一个可选值。为要按用途、所有者或环境等有用方式分类的 AWS 资源分配标签。为每个要添加的标签选择**添加新标签**。选择 “**下一步**” 继续工作流程。

  - **第 3 步选择自定义地点**

    在此步骤中，确定您计划添加到此队列中的计算机的物理位置。您现在可以指定一个或多个位置，也可以稍后根据需要添加或移除位置。在**自定义位置**中，为舰队的计算选择一个或多个位置。该列表包括在您当前选择的位置中定义的所有自定义位置 AWS 区域。要定义要添加到舰队的新自定义位置，请选择**创建位置**。选择 “**下一步**” 继续工作流程。

  - **第 4 步查看并创建**

    在创建队列之前，请检查您的设置。准备好部署新队列时，选择**创建**。Amazon GameLift 立即开始机队激活流程，分配一个唯一 ID 并将车队置于`NEW`状态。**您可以在舰队页面上跟踪舰队的进度。**

  

- AWS CLI

  使用[`create-fleet`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/create-fleet.html)命令创建计算类型的队列`ANYWHERE`。提供名称和至少一个自定义位置。Amazon 在您当前的默认值中 GameLift 创建 Anywhere 队列资源 AWS 区域 （或者您可以添加一个--region 标签来指定其他资源 AWS 区域）。

  以下示例请求使用最低要求的设置创建一个新队列。将 `FleetName` 和 `custom-location` 替换为您自己的信息。

  ```
  aws gamelift create-fleet \
  --name FleetName \
  --compute-type ANYWHERE \
  --locations "Location=custom-location"
  ```

  响应示例

  ```
  {
      "FleetAttributes": {
          "FleetId": "fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa",
          "FleetArn": "arn:aws:gamelift:us-west-2:111122223333:fleet/fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa",
          "Name": "HardwareAnywhere",
          "CreationTime": "2023-02-23T17:57:42.293000+00:00",
          "Status": "ACTIVE",
          "MetricGroups": [
              "default"
          ],
          "CertificateConfiguration": {
              "CertificateType": "DISABLED"
          },
          "ComputeType": "ANYWHERE"
      }
  }
  ```

  创作时，一个新的 Anywhere 舰队会迅速进入舰队状态`ACTIVE`。您可以在舰队到达`ACTIVE`队列后向其添加计算。

  请注意，响应中不包括舰队位置。您可以通过致电[`describe-fleet-attributes`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/describe-fleet-attributes.html)和来检索完整的车队详细信息[`describe-fleet-location-attributes`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/describe-fleet-location-attributes.html)。



## 向队列添加计算

要向队列中添加计算资源并使其做好托管游戏会话的准备，请执行以下任务：

- 向队列注册计算。注册会告知 Amazon GameLift 哪些物理托管资源是机群的一部分。
- 为计算请求身份验证令牌。在计算机上运行的每台游戏服务器都需要此令牌才能连接到 Amazon GameLift 服务。身份验证令牌是临时的，必须定期刷新。



###### 注意

如果您使用 Amazon A GameLift gent 部署游戏服务器软件，则可以跳过此步骤。代理会自动注册每台计算并为计算保留有效的身份验证令牌。请参阅 [与亚马逊 GameLift 代理合作](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-dev-iteration-agent.html)。

您可以使用 AWS CLI或对 Amazon 进行编程调用来注册计算并请求身份验证令牌 GameLift。 AWS SDK这些操作无法通过 Amazon GameLift 控制台进行。

作为最佳实践，我们建议通过向每台计算添加启动脚本来自动执行这两项任务。启动脚本会自动调用`register-compute`和`get-compute-auth-token`命令。您还可以自动执行任务，以便在计算的整个生命周期中定期刷新身份验证令牌，并在关闭时取消注册计算。

每个启动操作都会返回您需要存储在计算中的特定于计算的值。当游戏服务器进程在计算机上启动时，在初始化与 Amazon GameLift 服务的连接时，它必须将这些值作为服务器参数传递（参见[ServerParameters](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/integration-server-sdk5-cpp-datatypes.html#integration-server-sdk5-cpp-dataypes-serverparameters)服务器SDK参考资料）。我们建议您将这些特定于计算的值（或其存储位置）设置为环境变量。如果您使用的是 Amazon GameLift 代理，则此任务将由您处理。特定于计算的值如下所示：

- `register-compute`返回的值`GameLiftServiceSdkEndpoint`。将此值设置为`webSocketUrl`服务器参数。
- `compute-auth-token`返回身份验证令牌。将此值设置为`authToken`服务器参数。

- AWS CLI

以下说明描述了如何使用手动提交每个请求 AWS CLI。

**注册计算**

[`register-compute`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/register-compute.html)致电注册一台电脑。确定要向其添加计算的队列的 ID。提供以下计算信息：有意义的名称、IP 地址和位置。计算的位置必须是已与舰队关联的自定义位置。如果您想使用其他自定义位置，请使用 Amazon GameLift 控制台更新舰队或调用 AWS CLI命令[`create-fleet-locations`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/create-fleet-locations.html)向舰队添加自定义位置。

在以下示例中，替换计算和队列的占位符值。该`fleet-id`值将在您创建 Anywhere 队列时返回。您可以通过致电[`describe-fleet-attributes`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/describe-fleet-attributes.html)和来检索完整的车队详细信息[`describe-fleet-location-attributes`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/describe-fleet-location-attributes.html)。

```
aws gamelift register-compute \
    --compute-name HardwareAnywhere \
    --fleet-id arn:aws:gamelift:us-east-1:111122223333:fleet/fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa \
    --ip-address 10.1.2.3 \
    --location custom-location-1
```

示例输出

```
{
    "Compute": {
        "FleetId": "fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa",
        "FleetArn": "arn:aws:gamelift:us-west-2:111122223333:fleet/fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa",
        "ComputeName": "HardwareAnywhere",
        "ComputeArn": "arn:aws:gamelift:us-west-2:111122223333:compute/HardwareAnywhere",
        "IpAddress": "10.1.2.3",
        "ComputeStatus": "Active",
        "Location": "custom-location-1",
        "CreationTime": "2023-02-23T18:09:26.727000+00:00",
        "GameLiftServiceSdkEndpoint": "wss://us-west-2.api.amazongamelift.com"
    }
}
```

**请求身份验证令牌**

[`get-compute-auth-token`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/get-compute-auth-token.html)致电请求有效的身份验证令牌。注册一台计算。识别舰队 ID 和计算名称。

在以下示例中，替换计算和队列的占位符值。该`fleet-id`值将在您创建 Anywhere 队列时返回。您可以通过致电来检索完整的车队详细信息[`describe-fleet-attributes`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/describe-fleet-attributes.html)。要查找计算信息，请[`list-compute`](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/gamelift/list-compute.html)使用队列 ID 致电以查看注册到队列的所有计算。

```
aws gamelift get-compute-auth-token \
    --fleet-id arn:aws:gamelift:us-east-1:111122223333:fleet/fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa \
    --compute-name HardwareAnywhere
```

示例输出

```
{
    "FleetId": "fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa",
    "FleetArn": "arn:aws:gamelift:us-east-1:111122223333:fleet/fleet-2222bbbb-33cc-44dd-55ee-6666ffff77aa",
    "ComputeName": "HardwareAnywhere",
    "ComputeArn": "arn:aws:gamelift:us-east-1:111122223333:compute/HardwareAnywhere",
    "AuthToken": "0c728041-3e84-4aaa-b927-a0fb202684c0",
    "ExpirationTimestamp": "2023-02-23T18:47:54+00:00"
}
```



## 启动游戏服务器

在创建了 Anywhere 队列并向队列中添加了一台或多台计算机之后，就可以开始运行游戏服务器了。

- **第 1 步安装游戏服务器软件**

  将游戏服务器版本和所有相关软件安装到 Anywhere 队列中的每台计算机上。游戏服务器版本必须与 Amazon GameLift 服务器SDK版本 5.x（或更高版本）集成，并具备与亚马逊 GameLift 服务通信所需的最低功能。

- **第 2 步准备好运行游戏服务器的计算机**

  确保每台计算机都已注册并具有有效的身份验证令牌。如果您使用脚本来管理这些任务，请确保脚本在每台计算机上运行，然后再启动任何游戏服务器进程。如果您已在游戏服务器软件中部署了 Amazon A GameLift gent，请确保代理可执行文件已启动。

- **步骤 3 启动游戏服务器进程**

  在计算机上运行游戏服务器可执行文件的实例。如果您的游戏服务器版本已正确集成，则游戏服务器进程会`InitSDK()`使用一组有效的服务器参数调用服务器SDK操作。当服务器进程准备好托管游戏会话时，它会调用`ProcessReady()`。注意如果您使用 Amazon A GameLift gent 部署了游戏服务器软件，则可以跳过此步骤。代理会根据您提供的运行时说明自动启动游戏服务器进程。您可以通过查看激活服务器进程和活动服务器进程的服务器进程指标来监控进度。请参阅 [Amazon GameLift 实例集指标](https://docs.aws.amazon.com/zh_cn/gamelift/latest/developerguide/monitoring-cloudwatch.html#gamelift-metrics-fleet)。如果您的游戏服务器进程无法初始化，请确认该进程正在为其运行的计算检索正确的服务器参数值。