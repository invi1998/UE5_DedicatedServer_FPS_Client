# 什么是 AWS Lambda？

[原文链接](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/welcome.html)

您可以使用 AWS Lambda 运行代码而无需预置或管理服务器。

Lambda 在可用性高的计算基础设施上运行您的代码，执行计算资源的所有管理工作，其中包括服务器和操作系统维护、容量预置和弹性伸缩和记录。使用 Lambda，您只需在 Lambda 支持的一种语言运行时系统中提供代码。

您可以将代码组织到 Lambda 函数。Lambda 服务仅在需要时运行函数并自动扩展。您只需按使用的计算时间付费，代码未运行时不产生费用。有关更多信息，请参阅[AWS Lambda 定价](https://aws.amazon.com/lambda/pricing/)。



###### 提示

要了解如何构建**无服务器解决方案**，请查看[无服务器开发人员指南](https://docs.aws.amazon.com/serverless/latest/devguide/)。

## 何时使用 Lambda

Lambda 是一种理想的计算服务，适用于需要快速纵向扩展并在不需要时缩减至零的应用程序场景。例如，您可以在以下情况中使用 Lambda：

- **文件处理：**使用 Amazon Simple Storage Service（Amazon S3）在上传完毕后实时触发 Lambda 数据处理。
- **流处理：**使用 Lambda 和 Amazon Kinesis 处理实时流数据，用于应用程序活动跟踪、交易订单处理、点击流分析、数据清理、日志筛选、索引编制、社交媒体分析、物联网（IoT）设备数据遥测和计量。
- **Web 应用程序：**将 Lambda 与其他 AWS 服务相结合，构建功能强大的 Web 应用程序，这些应用程序可自动纵向扩展和缩减，并跨多个数据中心以高可用配置运行。
- **IoT 后端：**使用 Lambda 构建无服务器后端以处理 Web、移动设备、IoT 和第三方 API 请求。
- **移动后端：**使用 Lambda 和 Amazon API Gateway 构建后端以对 API 请求进行身份验证和处理 API 请求。使用 AWS Amplify 可轻松与 iOS、Android、Web 和 React Native 前端集成。

使用 Lambda 时，您只负责您的代码。Lambda 管理提供内存、CPU、网络和其他资源均衡的计算机群，以运行代码。由于 Lambda 管理这些资源，因此您无法在提供的运行时上登录计算实例或自定义操作系统。Lambda 代表您执行操作和管理活动，包括管理容量、监控并记录 Lambda 函数。

## 主要特征

以下关键功能可帮助您开发可扩展、安全且易于扩展的 Lambda 应用程序：

- **[环境变量](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/configuration-envvars.html)**

  使用环境变量来调整函数的行为，而无需更新代码。

- **[版本](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/configuration-versions.html)**

  使用版本管理函数部署，例如，可将新函数用于 beta 测试，而这不会影响稳定生产版本的用户。

- **[容器映像](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/images-create.html)**

  使用 AWS 提供的基本映像或备用基本映像为 Lambda 函数创建容器映像，以便重复使用现有的容器工具或部署依赖大量依赖项的较大工作负载，例如机器学习。

- **[图层](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/chapter-layers.html)**

  为库和其他依赖项打包，以减少部署存档的大小，并加快代码部署速度。

- **[Lambda 扩展](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/runtimes-extensions-api.html)**

  通过监控、可观测性、安全性和监管工具增强 Lambda 函数。

- **[函数 URL](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/urls-configuration.html)**

  向 Lambda 函数添加专用 HTTP（S）端点。

- **[响应流式处理](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/configuration-response-streaming.html)**

  配置 Lambda 函数 URL 以将响应负载从 Node.js 函数流式传输回客户端，以提高首字节时间（TTFB）性能或返回较大负载。

- **[并发和扩展控制](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-concurrency.html)**

  对生产应用程序的扩展和响应能力进行精细控制。

- **[代码签名](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/configuration-codesigning.html)**

  确认只有已获批准的开发人员才能发布 Lambda 函数中未经更改的可信代码

- **[私有联网](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/configuration-vpc.html)**

  为资源（如数据库、缓存实例或内部服务）创建专用网络。

- **[文件系统访问](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/configuration-filesystem.html)**

  配置函数以将 Amazon Elastic File System（Amazon EFS）挂载到本地目录，以便函数代码能够在高并发下安全地访问和修改共享资源。

- **[适用于 Java 的 Lambda SnapStart](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart.html)**

  将 **Java 运行时系统**的启动性能提高多达 10 倍，而无需支付额外费用，通常也无需更改函数代码。



# 了解 Lambda 执行环境生命周期

Lambda 在执行环境中调用您的函数，该环境提供一个安全和隔离的运行时环境。执行环境管理运行函数所需的资源。执行环境为函数的运行时以及与函数关联的任何[外部扩展](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-extensions.html)提供生命周期支持。

函数的运行时使用[运行时 API](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/runtimes-api.html) 与 Lambda 进行通信。扩展使用[扩展 API](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/runtimes-extensions-api.html) 与 Lambda 进行通信。扩展还可借助[遥测 API](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/telemetry-api.html)，从该函数接收日志消息与其他遥测数据。



![执行环境的体系结构图。](./telemetry-api-concept-diagram.png)



创建 Lambda 函数时，您需要指定配置信息，例如可用内存量和函数允许的最长执行时间。Lambda 使用此信息设置执行环境。

函数的运行时和每个外部扩展都是在执行环境中运行的进程。权限、资源、凭证和环境变量在函数和扩展之间共享。

###### 主题

- [Lambda 执行环境生命周期](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle)
- [在函数中实现无状态性](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#execution-environment-stateless)

## Lambda 执行环境生命周期

![Lambda 生命周期阶段：初始化、调用、关闭](./Overview-Successful-Invokes.png)



每个阶段都以 Lambda 发送到运行时和所有注册的扩展的事件开始。运行时和每个扩展通过发送 `Next` API 请求来指示完成。当运行时和每个扩展完成且没有挂起的事件时，Lambda 会冻结执行环境。

###### 主题

- [Init 阶段](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-ib)
- [在 Init 阶段失败](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-init-errors)
- [还原阶段（仅限 Lambda SnapStart）](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-restore)
- [调用阶段](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-invoke)
- [在调用阶段失败](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-invoke-with-errors)
- [关闭阶段](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-shutdown)

### Init 阶段

在 `Init` 阶段，Lambda 执行三项任务：

- 开启所有扩展 (`Extension init`)
- 引导运行时 (`Runtime init`)
- 运行函数的静态代码 (`Function init`)
- 运行任何 `beforeCheckpoint` [运行时挂钩](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart-runtime-hooks.html)（仅限 Lambda SnapStart）

当运行时和所有扩展通过发送 `Init` API 请求表明它们已准备就绪时， `Next` 阶段结束。`Init` 阶段限制为 10 秒。如果所有三个任务都未在 10 秒内完成，Lambda 在第一个函数调用时使用配置的函数超时值重试 `Init` 阶段。

激活 [Lambda SnapStart](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart.html) 后，在您发布一个函数版本时会发生 `Init` 阶段。Lambda 保存初始化的执行环境的内存和磁盘状态的快照，永久保存加密快照并对其进行缓存以实现低延迟访问。如果您具有 `beforeCheckpoint` [运行时挂钩](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart-runtime-hooks.html)，则该代码将在 `Init` 阶段结束时运行。



###### 注意

10 秒超时不适用于使用预调配并发或 SnapStart 的函数。对于预调配并发和 SnapStart 函数，初始化代码最长可能会运行 15 分钟。时间限制为 130 秒或配置的函数超时（最大 900 秒），以较高者为准。

使用[预置并发](https://docs.aws.amazon.com/lambda/latest/dg/provisioned-concurrency.html)时，Lambda 会在您为函数配置 PC 设置时初始化执行环境。Lambda 还确保初始化的执行环境在调用之前始终可用。您会发现函数的调用和初始化阶段之间存在时间差。根据函数的运行时系统和内存配置，在初始化的执行环境中首次调用时可能会发生一些延迟变化。

对于使用按需并发的函数，Lambda 可能会在调用请求之前偶尔初始化执行环境。发生这种情况时，您可能会观察到函数的初始化和调用阶段之间存在时间差。我们建议您不要依赖此行为。

### 在 Init 阶段失败

如果函数在 `Init` 阶段崩溃或超时，Lambda 会在日志中发出错误信息。`INIT_REPORT`

###### 例 — INIT_REPORT 超时日志

```
INIT_REPORT Init Duration: 1236.04 ms Phase: init Status: timeout
```

###### 例 — INIT_REPORT 扩展失败日志

```
INIT_REPORT Init Duration: 1236.04 ms Phase: init Status: error Error Type: Extension.Crash
```

如果 `Init` 阶段成功，除非激活 [SnapStart](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart.html)，否则 Lambda 不会发出 `INIT_REPORT` 日志。SnapStart 函数始终会发出 `INIT_REPORT`。有关更多信息，请参阅 [监控 Lambda SnapStart](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart-monitoring.html)。

### 还原阶段（仅限 Lambda SnapStart）

当您首次调用 [SnapStart](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart.html) 函数时，随着该函数的扩展，Lambda 会从永久保存的快照中恢复新的执行环境，而不是从头开始初始化函数。如果您有 `afterRestore()` [运行时挂钩](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart-runtime-hooks.html)，则代码将在 `Restore` 阶段结束时运行。`afterRestore()` 运行时挂钩执行期间将产生费用。必须加载运行时（JVM），并且 `afterRestore()` 运行时挂钩必须在超时限制（10 秒）内完成。否则，您将收到 SnapStartTimeoutException。`Restore` 阶段完成后，Lambda 将调用函数处理程序（[调用阶段](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#runtimes-lifecycle-invoke)）。

#### 在 Restore 阶段失败

如果 `Restore` 阶段失败，Lambda 会在 `RESTORE_REPORT` 日志中发出错误信息。

###### 例 — RESTORE_REPORT 超时日志

```
RESTORE_REPORT Restore Duration: 1236.04 ms Status: timeout
```

###### 例 — RESTORE_REPORT 运行时系统钩子失败日志

```
RESTORE_REPORT Restore Duration: 1236.04 ms Status: error Error Type: Runtime.ExitError
```

有关 `RESTORE_REPORT` 日志的更多信息，请参阅 [监控 Lambda SnapStart](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/snapstart-monitoring.html)。

### 调用阶段

当调用 Lambda 函数以响应 `Next` API 请求时，Lambda 向运行时和每个扩展发送一个 `Invoke` 事件。

函数的超时设置限制了整个 `Invoke` 阶段的持续时间。例如，如果将函数超时设置为 360 秒，则该函数和所有扩展都需要在 360 秒内完成。请注意，没有独立的调用后阶段。持续时间是所有调用时间（运行时 + 扩展）的总和，直到函数和所有扩展完成执行之后才计算。

调用阶段在运行时之后结束，所有扩展都通过发送 `Next` API 表示它们已完成。

### 在调用阶段失败

如果 Lambda 函数在 `Invoke` 阶段崩溃或超时，Lambda 会重置执行环境。下图演示了调用失败时的 Lambda 执行环境行为：

![执行环境示例：初始化、调用、调用时出错、调用、关闭](./Overview-Invoke-with-Error.png)



在上图中：

- 第一个阶段是 **INIT** 阶段，运行没有错误。

- 第二个阶段是 **INVOKE** 阶段，运行没有错误。

- 假设您的函数在某个时点遇到调用失败的问题（例如函数超时或运行时错误）。标签为 **INVOKE WITH ERROR** 的第三个阶段演示了这种情况。出现这种情况时，Lambda 服务会执行重置。重置的行为类似于 `Shutdown` 事件。首先，Lambda 会关闭运行时，然后向每个注册的外部扩展发送一个 `Shutdown` 事件。该事件包括关闭的原因。如果此环境用于新调用，则 Lambda 会将扩展和运行时与下一次调用一起重新初始化。

  请注意，Lambda 重置不会在下一个初始化阶段之前清除 `/tmp` 目录内容。这种行为与常规关闭阶段一致。

  

  ###### 注意

  AWS 目前正在实施对 Lambda 服务的更改。由于这些更改，您可能会看到 AWS 账户 中不同 Lambda 函数发出的系统日志消息和跟踪分段的结构和内容之间存在细微差异。

  如果您的函数的系统日志配置设置为纯文本，则当您的函数遇到调用失败时，此更改会影响在 CloudWatch Logs 中捕获的日志消息。以下示例显示了新旧格式的日志输出。

  这些更改将在未来几周内实施，除中国和 GovCloud 区域外，所有 AWS 区域 的函数都将过渡到使用新格式的日志消息和跟踪分段。

  

  ###### 例 CloudWatch Logs 日志输出（运行时或扩展崩溃）（旧样式）

  ```
  START RequestId: c3252230-c73d-49f6-8844-968c01d1e2e1 Version: $LATEST
  RequestId: c3252230-c73d-49f6-8844-968c01d1e2e1 Error: Runtime exited without providing a reason
  Runtime.ExitError
  END RequestId: c3252230-c73d-49f6-8844-968c01d1e2e1
  REPORT RequestId: c3252230-c73d-49f6-8844-968c01d1e2e1 Duration: 933.59 ms Billed Duration: 934 ms Memory Size: 128 MB Max Memory Used: 9 MB
  ```

  ###### 例 CloudWatch Logs 日志输出（函数超时）（旧样式）

  ```
  START RequestId: b70435cc-261c-4438-b9b6-efe4c8f04b21 Version: $LATEST
  2024-03-04T17:22:38.033Z b70435cc-261c-4438-b9b6-efe4c8f04b21 Task timed out after 3.00 seconds
  END RequestId: b70435cc-261c-4438-b9b6-efe4c8f04b21
  REPORT RequestId: b70435cc-261c-4438-b9b6-efe4c8f04b21 Duration: 3004.92 ms Billed Duration: 3000 ms Memory Size: 128 MB Max Memory Used: 33 MB Init Duration: 111.23 ms
  ```

  CloudWatch 日志的新格式在 `REPORT` 行中包含一个附加 `status` 字段。在运行时或扩展崩溃的情况下，`REPORT` 行还包含一个字段 `ErrorType`。

  

  ###### 例 CloudWatch Logs 日志输出（运行时或扩展崩溃）（新样式）

  ```
  START RequestId: 5b866fb1-7154-4af6-8078-6ef6ca4c2ddd Version: $LATEST
  END RequestId: 5b866fb1-7154-4af6-8078-6ef6ca4c2ddd
  REPORT RequestId: 5b866fb1-7154-4af6-8078-6ef6ca4c2ddd Duration: 133.61 ms Billed Duration: 133 ms Memory Size: 128 MB Max Memory Used: 31 MB Init Duration: 80.00 ms Status: error Error Type: Runtime.ExitError
  ```

  ###### 例 CloudWatch Logs 日志输出（函数超时）（新样式）

  ```
  START RequestId: 527cb862-4f5e-49a9-9ae4-a7edc90f0fda Version: $LATEST
  END RequestId: 527cb862-4f5e-49a9-9ae4-a7edc90f0fda
  REPORT RequestId: 527cb862-4f5e-49a9-9ae4-a7edc90f0fda Duration: 3016.78 ms Billed Duration: 3016 ms Memory Size: 128 MB Max Memory Used: 31 MB Init Duration: 84.00 ms Status: timeout
  ```

- 第四个阶段是调用失败后立即进入的 **INVOKE** 阶段。在这里，Lambda 通过重新运行 **INIT** 阶段重新初始化环境。此情况称为*隐藏初始化*。出现隐藏初始化时，Lambda 不会在 CloudWatch Logs 中显式报告额外的 **INIT** 阶段。相反，您可能会注意到 REPORT 行中的持续时间包括一个额外的 **INIT** 持续时间 + **INVOKE** 持续时间。例如，假设您在 CloudWatch 中看到以下日志：

  ```
  2022-12-20T01:00:00.000-08:00 START RequestId: XXX Version: $LATEST 
  2022-12-20T01:00:02.500-08:00 END RequestId: XXX 
  2022-12-20T01:00:02.500-08:00 REPORT RequestId: XXX Duration: 3022.91 ms 
  Billed Duration: 3000 ms Memory Size: 512 MB Max Memory Used: 157 MB
  ```

  在此例中，REPORT 和 START 时间戳的间隔为 2.5 秒。这与报告的持续时间（3022.91 毫秒）不一致，因为它没有考虑 Lambda 执行的额外 **INIT**（隐藏初始化）。在此例中，您可以推断出实际的 **INVOKE** 阶段用时 2.5 秒。

  要更深入地了解这种行为，您可以使用 [使用遥测 API 访问扩展的实时遥测数据](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/telemetry-api.html)。每当在调用阶段之间出现隐藏初始化时，Telemetry API 都会发出 `INIT_START`、`INIT_RUNTIME_DONE`、`INIT_REPORT` 事件以及 `phase=invoke`。

- 第五个阶段是 **SHUTDOWN** 阶段，该阶段运行没有错误。

### 关闭阶段

若 Lambda 即将关闭运行时，它会向每个已注册的外部扩展发送一个 `Shutdown` 事件。扩展可以使用此时间执行最终清理任务。`Shutdown` 事件是对 `Next` API 请求的响应。

**持续时间**：整个 `Shutdown` 阶段的上限为 2 秒。如果运行时或任何扩展没有响应，则 Lambda 会通过一个信号 (`SIGKILL`) 终止它。

在函数和所有扩展完成后，Lambda 维护执行环境一段时间，以预期另一个函数调用。但是，Lambda 每隔几个小时就会终止执行环境，以便进行运行时更新和维护，即使是连续调用的函数亦不例外。您不应假设执行环境将无限期持续。有关更多信息，请参阅 [在函数中实现无状态性](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/lambda-runtime-environment.html#execution-environment-stateless)。

当再次调用该函数时，Lambda 会解冻环境以便重复使用。重复使用执行环境会产生以下影响：

- 在该函数的处理程序方法的外部声明的对象保持已初始化的状态，再次调用函数时提供额外的优化功能。例如，如果您的 Lambda 函数建立数据库连接，而不是重新建立连接，则在后续调用中使用原始连接。建议您在代码中添加逻辑，以便在创建新连接之前检查是否存在连接。
- 每个执行环境都在 `/tmp` 目录中提供 512MB 到 10240MB 之间的磁盘空间（以 1MB 递增）。冻结执行环境时，目录内容会保留，同时提供可用于多次调用的暂时性缓存。您可以添加额外的代码来检查缓存中是否有您存储的数据。有关部署大小限制的更多信息，请参阅[Lambda 配额](https://docs.aws.amazon.com/zh_cn/lambda/latest/dg/gettingstarted-limits.html)：
- 如果 Lambda 重复使用执行环境，则由 Lambda 函数启动但在函数结束时未完成的后台进程或回调将继续执行。确保代码中的任何后台进程或回调在代码退出前已完成。

## 在函数中实现无状态性

在编写 Lambda 函数代码时，应视执行环境无状态，即假定执行环境仅在单次调用中存在。Lambda 每隔几个小时就会终止执行环境，以便进行运行时更新和维护，即使是连续调用的函数亦不例外。请在函数启动时初始化任何要求的状态（例如，从 Amazon DynamoDB 表中获取购物车）。在退出之前，请将永久数据更改提交给持久存储，如 Amazon Simple Storage Service（Amazon S3）、DynamoDB 或 Amazon Simple Queue Service（Amazon SQS）。避免依赖跨多个调用的现有数据结构、临时文件或状态，例如计数器或聚合。这样可以确保函数能够独立处理每次调用。