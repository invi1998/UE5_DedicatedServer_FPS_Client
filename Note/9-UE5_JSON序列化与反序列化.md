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