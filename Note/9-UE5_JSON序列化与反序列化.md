# UE5里JSON序列化和反序列化



在Unreal Engine 5（UE5）中，序列化和反序列化JSON数据通常涉及到使用`FJsonObject`类来处理JSON对象以及`FJsonSerializer`类来进行序列化和反序列化的操作。下面将详细解释这个过程，并提供一些代码示例。

### 序列化（从C++对象到JSON）

1. **创建一个包含你的数据的结构体或类**，并确保它能够被序列化。你需要为这些类型实现`CustomJsonSerializer`，或者直接使用`TSharedPtr<FJsonValue>`进行封装。

2. **创建一个`TSharedRef<TJsonWriter<>>`实例**，用于写入JSON格式的数据。

3. **创建一个`FJsonObject`实例**，并向其中添加键值对，代表你要序列化的数据。

4. **使用`FJsonSerializer::Serialize`方法**，将`FJsonObject`序列化为JSON格式的字符串。

以下是一个简单的序列化例子：

```cpp
void SerializeToJSON(const FString& FilePath)
{
    // 创建一个空的 JSON 对象。
    TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    // 向 JSON 对象添加键值对。
    JsonObject->SetStringField(TEXT("Name"), TEXT("John Doe"));
    JsonObject->SetNumberField(TEXT("Age"), 30);

    // 创建一个文件流写入器。
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&FilePath);

    // 将 JSON 对象序列化为 JSON 格式的字符串。
    if (FJsonSerializer::Serialize(JsonObject, Writer))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully serialized to JSON."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to serialize to JSON."));
    }

    // 关闭写入器。
    Writer.Flush();
}
```

### 反序列化（从JSON到C++对象）

1. **读取JSON格式的字符串**，这可能是从文件、网络或其他来源获取的。

2. **创建一个`TSharedRef<TJsonReader<>>`实例**，用于解析JSON格式的数据。

3. **创建一个`TSharedPtr<FJsonObject>`实例**，用于存储解析后的JSON对象。

4. **使用`FJsonSerializer::Deserialize`方法**，将JSON格式的字符串反序列化为`FJsonObject`。

5. **从`FJsonObject`中提取数据**，并将其赋值给你的C++对象。

以下是反序列化的一个简单例子：

```cpp
void DeserializeFromJSON(const FString& JsonString)
{
    // 创建一个 JSON 读取器。
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    // 创建一个空的 JSON 对象指针。
    TSharedPtr<FJsonObject> JsonObject;

    // 将 JSON 字符串反序列化为 JSON 对象。
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // 从 JSON 对象中提取数据。
        FString Name;
        int32 Age;

        if (JsonObject->TryGetStringField(TEXT("Name"), Name) &&
            JsonObject->TryGetNumberField(TEXT("Age"), Age))
        {
            UE_LOG(LogTemp, Log, TEXT("Name: %s, Age: %d"), *Name, Age);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to extract all fields from JSON."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
    }
}
```

请注意，以上代码中的`TJsonWriterFactory<>`和`TJsonReaderFactory<>`默认是用UTF-8编码的。如果你需要不同的编码，请指定模板参数。



# FJsonObjectConverter::JsonObjectToUStruct

`FJsonObjectConverter::JsonObjectToUStruct` 是 Unreal Engine 中用于将 JSON 对象转换为 C++ 结构体（USTRUCT）的一个工具函数。它简化了从 JSON 数据到 Unreal Engine 结构或类的反序列化过程，允许开发者轻松地将 JSON 数据映射到他们定义的数据结构中。

### 使用 `FJsonObjectConverter::JsonObjectToUStruct`

为了使用这个函数，你需要有一个有效的 `TSharedPtr<FJsonObject>` 和一个目标 USTRUCT 类型的指针。该函数会尝试将 JSON 对象中的数据映射到给定的 USTRUCT 类型，并返回是否成功。

#### 函数签名

```cpp
template<typename StructType>
bool FJsonObjectConverter::JsonObjectToUStruct(
    const TSharedPtr<FJsonObject>& JsonObject,
    StructType* OutStruct,
    bool SetDefaults = false,
    bool AllowUnknownFields = true,
    int32 ArrayIndex = INDEX_NONE,
    UScriptStruct* StructDefinition = nullptr);
```

- `JsonObject`: 你想要转换的JSON对象。
- `OutStruct`: 输出的目标USTRUCT类型的指针，这是JSON数据将被填充的地方。
- `SetDefaults`: 如果设置为true，那么对于JSON对象中不存在的字段，将会使用默认值初始化。
- `AllowUnknownFields`: 如果设置为true，那么JSON中存在但USTRUCT中不存在的字段不会导致错误。
- `ArrayIndex`: 当处理数组时，可以指定元素的索引；通常情况下你可以忽略这个参数。
- `StructDefinition`: 这个参数通常是可选的，除非你需要明确指定一个UScriptStruct类型。

### 示例代码

下面是一个简单的例子，展示如何使用 `FJsonObjectConverter::JsonObjectToUStruct` 将 JSON 对象转换为自定义的 USTRUCT：

首先，定义一个 USTRUCT：

```cpp
USTRUCT(BlueprintType)
struct FMyData
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    int32 Age;
};
```

然后，使用 `FJsonObjectConverter::JsonObjectToUStruct` 进行转换：

```cpp
void DeserializeFromJSON(const FString& JsonString)
{
    // 创建一个 JSON 读取器。
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    // 创建一个空的 JSON 对象指针。
    TSharedPtr<FJsonObject> JsonObject;

    // 将 JSON 字符串反序列化为 JSON 对象。
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // 创建一个目标 USTRUCT 的实例。
        FMyData MyData;

        // 使用 FJsonObjectConverter 将 JSON 对象转换为 USTRUCT。
        if (FJsonObjectConverter::JsonObjectToUStruct<FMyData>(JsonObject.ToSharedRef(), &MyData, /*SetDefaults=*/ true))
        {
            UE_LOG(LogTemp, Log, TEXT("Name: %s, Age: %d"), *MyData.Name, MyData.Age);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to convert JSON object to USTRUCT."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
    }
}
```

在上面的例子中，我们首先解析了一个 JSON 字符串为 `FJsonObject`，然后我们创建了一个 `FMyData` 实例，并调用了 `FJsonObjectConverter::JsonObjectToUStruct` 来填充这个实例。如果转换成功，我们将打印出 USTRUCT 中的值；如果不成功，则记录一个错误消息。

确保你的 USTRUCT 定义了 `GENERATED_BODY()` 宏，并且所有你想通过 JSON 反序列化的成员变量都标记有 `UPROPERTY()` 宏。这使得这些成员能够被 `FJsonObjectConverter` 正确识别和处理。