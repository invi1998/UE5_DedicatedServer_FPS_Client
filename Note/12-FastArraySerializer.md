# Array Replication

> 该功能包含在 NetCore 模块中

在Unreal Engine 5 (UE5)中，Array Replication（数组复制）和Fast Array Serializer（快速数组序列化器）是与网络编程和数据同步相关的两个特性。它们用于确保在网络游戏中，客户端和服务器之间的复杂数据结构如数组能够高效、准确地进行同步。

### Array Replication

在多人游戏开发中，网络复制是一个关键概念，它涉及将一个玩家的动作或状态变化从服务器发送到所有相关客户端，反之亦然。对于简单的变量，比如整数或浮点数，这通常比较直接。但是，对于动态数组等复杂的数据结构，事情就变得复杂了，因为需要考虑效率和带宽使用的问题。

UE5中的Array Replication允许开发者标记出哪些数组应该在网络之间进行复制。引擎会处理这些数组的变更检测，并只在网络上传输那些实际发生变化的数据，而不是每次都传输整个数组。这有助于减少不必要的网络流量，提高性能。

要实现数组的网络复制，你通常会在你的Actor或子对象类中定义一个带有`UPROPERTY`宏的数组属性，并设置`ReplicatedUsing`参数指定一个函数来处理这个数组的变更通知。

```cpp
UPROPERTY(ReplicatedUsing = OnRep_MyArray)
TArray<int32> MyArray;
```

然后你需要实现`OnRep_MyArray`函数来响应数组的变化。

### Fast Array Serializer

Fast Array Serializer是一种优化技术，旨在进一步提升数组在网络上的复制效率。它解决了传统方法中的一些低效问题，特别是在处理大型或频繁更新的数组时。

Fast Array Serializer的主要优点在于它可以更有效地打包和解包数组内容，减少了序列化和反序列化的时间开销。它还支持增量更新，即只发送自上次更新以来发生变化的元素，而非整个数组。

为了利用Fast Array Serializer，你需要确保你的类继承自`FArraySerializer`并正确地实现了必要的接口函数。UE4引入了这个特性，而UE5继续支持并可能进行了改进。

需要注意的是，具体的API和最佳实践可能会随着引擎版本的不同而有所变化，因此始终建议查阅最新的官方文档或示例代码以获得最准确的信息。如果你正在从事UE5项目，确保你的项目设置和代码符合当前版本的最佳实践。

```C++
/**
 *	===================== Fast TArray Replication ===================== 
 *
 *	Fast TArray Replication is a custom implementation of NetDeltaSerialize that is suitable for TArrays of UStructs. It offers performance
 *	improvements for large data sets, it serializes removals from anywhere in the array optimally, and allows events to be called on clients
 *	for adds and removals. The downside is that you will need to have game code mark items in the array as dirty, and well as the *order* of the list
 *	is not guaranteed to be identical between client and server in all cases.
 *
 *	Using FTR is more complicated, but this is the code you need:
 *
 */
#if 0
	
/** Step 1: Make your struct inherit from FFastArraySerializerItem */
USTRUCT()
struct FExampleItemEntry : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY()

	// Your data:
	UPROPERTY()
	int32		ExampleIntProperty;	

	UPROPERTY()
	float		ExampleFloatProperty;


	/** 
	 * Optional functions you can implement for client side notification of changes to items; 
	 * Parameter type can match the type passed as the 2nd template parameter in associated call to FastArrayDeltaSerialize
	 * 
	 * NOTE: It is not safe to modify the contents of the array serializer within these functions, nor to rely on the contents of the array 
	 * being entirely up-to-date as these functions are called on items individually as they are updated, and so may be called in the middle of a mass update.
	 */
	void PreReplicatedRemove(const struct FExampleArray& InArraySerializer);
	void PostReplicatedAdd(const struct FExampleArray& InArraySerializer);
	void PostReplicatedChange(const struct FExampleArray& InArraySerializer);

	// Optional: debug string used with LogNetFastTArray logging
	FString GetDebugString();

};

/** Step 2: You MUST wrap your TArray in another struct that inherits from FFastArraySerializer */
USTRUCT()
struct FExampleArray: public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FExampleItemEntry>	Items;	/** Step 3: You MUST have a TArray named Items of the struct you made in step 1. */

	/** Step 4: Copy this, replace example with your names */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
	   return FFastArraySerializer::FastArrayDeltaSerialize<FExampleItemEntry, FExampleArray>( Items, DeltaParms, *this );
	}
};

/** Step 5: Copy and paste this struct trait, replacing FExampleArray with your Step 2 struct. */
template<>
struct TStructOpsTypeTraits< FExampleArray > : public TStructOpsTypeTraitsBase2< FExampleArray >
{
       enum 
       {
			WithNetDeltaSerializer = true,
       };
};

#endif

/** Step 6 and beyond: 
 *		-Declare a UPROPERTY of your FExampleArray (step 2) type.
 *		-You MUST call MarkItemDirty on the FExampleArray when you change an item in the array. You pass in a reference to the item you dirtied. 
 *			See FFastArraySerializer::MarkItemDirty.
 *		-You MUST call MarkArrayDirty on the FExampleArray if you remove something from the array.
 *		-In your classes GetLifetimeReplicatedProps, use DOREPLIFETIME(YourClass, YourArrayStructPropertyName);
 *
 *		You can provide the following functions in your structure (step 1) to get notifies before add/deletes/removes:
 *			-void PreReplicatedRemove(const FFastArraySerializer& Serializer)
 *			-void PostReplicatedAdd(const FFastArraySerializer& Serializer)
 *			-void PostReplicatedChange(const FFastArraySerializer& Serializer)
 *			-void PostReplicatedReceive(const FFastArraySerializer::FPostReplicatedReceiveParameters& Parameters)
 *
 *		Thats it!
 */ 




/**
 *	
 *	===================== An Overview of Net Serialization and how this all works =====================
 *
 *		Everything originates in UNetDriver::ServerReplicateActors.
 *		Actors are chosen to replicate, create actor channels, and UActorChannel::ReplicateActor is called.
 *		ReplicateActor is ultimately responsible for deciding what properties have changed, and constructing an FOutBunch to send to clients.
 *
 *	The UActorChannel has 2 ways to decide what properties need to be sent.
 *		The traditional way, which is a flat TArray<uint8> buffer: UActorChannel::Recent. This represents a flat block of the actor properties.
 *		This block literally can be cast to an AActor* and property values can be looked up if you know the FProperty offset.
 *		The Recent buffer represents the values that the client using this actor channel has. We use recent to compare to current, and decide what to send.
 *
 *		This works great for 'atomic' properties; ints, floats, object*, etc.
 *		It does not work for 'dynamic' properties such as TArrays, which store values Num/Max but also a pointer to their array data,
 *		The array data has no where to fit in the flat ::Recent buffer. (Dynamic is probably a bad name for these properties)
 *
 *		To get around this, UActorChannel also has a TMap for 'dynamic' state. UActorChannel::RecentDynamicState. This map allows us to look up
 *		a 'base state' for a property given a property's RepIndex.
 *
 *	NetSerialize & NetDeltaSerialize
 *		Properties that fit into the flat Recent buffer can be serialized entirely with NetSerialize. NetSerialize just reads or writes to an FArchive.
 *		Since the replication can just look at the Recent[] buffer and do a direct comparison, it can tell what properties are dirty. NetSerialize just
 *		reads or writes.
 *
 *		Dynamic properties can only be serialized with NetDeltaSerialize. NetDeltaSerialize is serialization from a given base state, and produces
 *		both a 'delta' state (which gets sent to the client) and a 'full' state (which is saved to be used as the base state in future delta serializes).
 *		NetDeltaSerialize essentially does the diffing as well as the serialization. It must do the diffing so it can know what parts of the property it must
 *		send.
 *	
 *	Base States and dynamic properties replication.
 *		As far as the replication system / UActorChannel is concerned, a base state can be anything. The base state only deals with INetDeltaBaseState*.
 *
 *		UActorChannel::ReplicateActor will ultimately decide whether to call FProperty::NetSerializeItem or FProperty::NetDeltaSerializeItem.
 *
 *		As mentioned above NetDeltaSerialize takes in an extra base state and produces a diff state and a full state. The full state produced is used
 *		as the base state for future delta serialization. NetDeltaSerialize uses the base state and the current values of the actor to determine what parts
 *		it needs to send.
 *		
 *		The INetDeltaBaseStates are created within the NetDeltaSerialize functions. The replication system / UActorChannel does not know about the details.
 *
 *		Right now, there are 2 forms of delta serialization: Generic Replication and Fast Array Replication.
 *
 *	
 *	Generic Delta Replication
 *		Generic Delta Replication is implemented by FStructProperty::NetDeltaSerializeItem, FArrayProperty::NetDeltaSerializeItem, FProperty::NetDeltaSerializeItem.
 *		It works by first NetSerializing the current state of the object (the 'full' state) and using memcmp to compare it to previous base state. FProperty
 *		is what actually implements the comparison, writing the current state to the diff state if it has changed, and always writing to the full state otherwise.
 *		The FStructProperty and FArrayProperty functions work by iterating their fields or array elements and calling the FProperty function, while also embedding
 *		meta data. 
 *
 *		For example FArrayProperty basically writes: 
 *			"Array has X elements now" -> "Here is element Y" -> Output from FProperty::NetDeltaSerialize -> "Here is element Z" -> etc
 *
 *		Generic Data Replication is the 'default' way of handling FArrayProperty and FStructProperty serialization. This will work for any array or struct with any 
 *		sub properties as long as those properties can NetSerialize.
 *
 *	Custom Net Delta Serialiation
 *		Custom Net Delta Serialiation works by using the struct trait system. If a struct has the WithNetDeltaSerializer trait, then its native NetDeltaSerialize
 *		function will be called instead of going through the Generic Delta Replication code path in FStructProperty::NetDeltaSerializeItem.
 *
 *	Fast TArray Replication
 *		Fast TArray Replication is implemented through custom net delta serialization. Instead of a flat TArray buffer to represent states, it only is concerned
 *		with a TMap of IDs and ReplicationKeys. The IDs map to items in the array, which all have a ReplicationID field defined in FFastArraySerializerItem.
 *		FFastArraySerializerItem also has a ReplicationKey field. When items are marked dirty with MarkItemDirty, they are given a new ReplicationKey, and assigned
 *		a new ReplicationID if they don't have one.
 *
 *		FastArrayDeltaSerialize (defined below)
 *		During server serialization (writing), we compare the old base state (e.g, the old ID<->Key map) with the current state of the array. If items are missing
 *		we write them out as deletes in the bunch. If they are new or changed, they are written out as changed along with their state, serialized via a NetSerialize call.
 *
 *		For example, what actually is written may look like:
 *			"Array has X changed elements, Y deleted elements" -> "element A changed" -> Output from NetSerialize on rest of the struct item -> "Element B was deleted" -> etc
 *
 *		Note that the ReplicationID is replicated and in sync between client and server. The indices are not.
 *
 *		During client serialization (reading), the client reads in the number of changed and number of deleted elements. It also builds a mapping of ReplicationID -> local index of the current array.
 *		As it deserializes IDs, it looks up the element and then does what it needs to (create if necessary, serialize in the current state, or delete).
 *
 *		Delta Serialization for inner structs is now enabled by default. That means that when a ReplicationKey changes, we will compare the current state of the
 *		struct to the last sent state, tracking changelists and only sending properties that changed exactly like the standard replication path.
 *		If this causes issues with a specific FastArray type, it can be disabled by calling FFastArraySerializer::SetDeltaSerializationEnabled(false) in the constructor.
 *		The feature can be completely disabled by setting the "net.SupportFastArrayDelta" CVar to 0.
 *
 *		ReplicationID and ReplicationKeys are set by the MarkItemDirty function on FFastArraySerializer. These are just int32s that are assigned in order as things change.
 *		There is nothing special about them other than being unique.
 */
```



Fast TArray Replication 是 Unreal Engine 5 中的一种优化技术，用于高效地序列化和同步大型数据集中的 `TArray` 结构体数组。这种技术特别适用于需要频繁更新的大型数组，因为它可以更有效地处理数组元素的添加、删除和修改操作。

以下是 Fast TArray Replication 的详细步骤和原理：

### 步骤概述

1. **继承 FFastArraySerializerItem**
   - 创建一个自定义结构体，并让它继承自 `FFastArraySerializerItem`。
   - 在这个结构体内定义你想要同步的数据成员。
   - 可选地实现一些回调函数（如 `PreReplicatedRemove`、`PostReplicatedAdd` 等），以便在客户端接收到数组的变化时执行特定的操作。

2. **包装 TArray 到 FFastArraySerializer**
   - 创建一个新的结构体来封装你的 `TArray`，并让这个新结构体继承自 `FFastArraySerializer`。
   - 必须在新结构体中包含一个名为 `Items` 的 `TArray` 成员，其类型是你第一步中创建的结构体。
   - 实现 `NetDeltaSerialize` 函数，调用 `FastArrayDeltaSerialize` 方法来进行高效的序列化。

3. **启用 NetDeltaSerializer 特性**
   - 为你的新结构体提供模板特化，设置 `WithNetDeltaSerializer` 标志为 `true`，以指示引擎使用自定义的序列化方法。

4. **声明 UPROPERTY 和管理脏标记**
   - 在你的类中声明一个 `UPROPERTY`，类型为你第二步中创建的结构体。
   - 当你更改数组中的某个元素时，必须调用 `MarkItemDirty` 来标记该元素为“脏”状态。
   - 如果从数组中移除元素，则需要调用 `MarkArrayDirty`。
   - 在类的 `GetLifetimeReplicatedProps` 函数中，使用 `DOREPLIFETIME` 宏注册你要复制的属性。

### 原理解释

#### 差异化序列化
- **传统方式**: 对于简单的原子属性（如整数、浮点数等），Unreal Engine 使用传统的 `NetSerialize` 方法，通过比较当前值与最近一次发送给客户端的值来决定哪些属性需要发送。
- **差异化序列化 (NetDeltaSerialize)**: 对于动态属性（如 `TArray`），无法直接存储在扁平化的缓冲区中。因此，Unreal Engine 使用差异化的序列化方法，只发送那些发生变化的部分。这对于大型数组尤其有效。

#### 基础状态管理
- **基础状态**: 每个动态属性都有一个基础状态，用于记录上次发送给客户端的状态。当需要进行差异化序列化时，引擎会将当前状态与基础状态进行比较，找出变化的部分。
- **ID 和 Key 映射**: Fast TArray Replication 使用 ID 和 Key 的映射来跟踪数组中的每个元素。当元素被标记为“脏”或被删除时，这些信息会被编码到网络包中，并在客户端进行相应的更新。

#### 示例代码解释
以下是一个简化的示例代码，展示了如何实现 Fast TArray Replication:

```C++
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Serialization/FastArraySerializer.h"

USTRUCT()
struct FExampleItemEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

    // 数据成员
    UPROPERTY(Replicated)
    int32 ExampleIntProperty;

    UPROPERTY(Replicated)
    float ExampleFloatProperty;

    // 可选回调函数
    void PreReplicatedRemove(const struct FExampleArray& InArraySerializer);
    void PostReplicatedAdd(const struct FExampleArray& InArraySerializer);
    void PostReplicatedChange(const struct FExampleArray& InArraySerializer);

    FString GetDebugString() const;
};

USTRUCT()
struct FExampleArray : public FFastArraySerializer
{
    GENERATED_BODY()

    // 包含要同步的项的数组
    UPROPERTY(Replicated)
    TArray<FExampleItemEntry> Items;

    bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms) override
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FExampleItemEntry, FExampleArray>(Items, DeltaParms, *this);
    }
};

// 启用 NetDeltaSerializer 特性
template<>
struct TStructOpsTypeTraits<FExampleArray> : public TStructOpsTypeTraitsBase2<FExampleArray>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};

```



在这个示例中，我们定义了一个 `FExampleItemEntry` 结构体，它继承自 `FFastArraySerializerItem` 并包含了两个可复制的属性。然后，我们创建了一个 `FExampleArray` 结构体来封装这个 `TArray`，并实现了必要的 `NetDeltaSerialize` 方法。最后，我们启用了 `NetDeltaSerializer` 特性，以便引擎知道使用我们的自定义序列化方法。

通过这种方式，你可以显著提高大型数组的同步性能，减少带宽消耗，并确保客户端和服务器之间的数据一致性。



`NetDeltaSerialize` 是 Unreal Engine 中用于网络同步的一个关键函数，特别是在处理动态数据结构（如 `TArray`）时。它的主要目的是在客户端和服务器之间高效地传输数据变化，而不是每次都发送整个数据集。这样可以显著减少网络流量，并提高性能。

### `NetDeltaSerialize` 的作用

1. **差异化序列化**:
   - 传统的 `NetSerialize` 方法会在每次更新时将整个数据集序列化并发送给客户端。
   - `NetDeltaSerialize` 只会序列化自上次发送以来发生变化的部分数据，从而节省带宽。

2. **基础状态管理**:
   - 维护一个“基础状态”，即上一次发送给客户端的数据快照。
   - 在进行差异化序列化时，将当前状态与基础状态进行比较，找出差异部分。

3. **增量更新**:
   - 发送新增、修改和删除的元素，而不是整个数组。
   - 这种方式特别适用于大型数组，因为只需要传输变化的部分。

### `NetDeltaSerialize` 的实现

在 Fast TArray Replication 中，`NetDeltaSerialize` 函数通常通过调用 `FastArrayDeltaSerialize` 来实现。以下是对这个过程的详细解释：

#### 示例代码中的 `NetDeltaSerialize`

让我们回到之前的示例代码，看看 `NetDeltaSerialize` 是如何实现的：



#### 详细步骤

1. **继承 `FFastArraySerializerItem`**:
   - `FExampleItemEntry` 结构体继承自 `FFastArraySerializerItem`，这使得它可以被包含在 `FFastArraySerializer` 中进行高效的网络同步。

2. **定义数据成员**:
   - `ExampleIntProperty` 和 `ExampleFloatProperty` 是需要在网络中同步的属性。
   - 使用 `UPROPERTY(Replicated)` 宏标记这些属性为可复制属性。

3. **实现 `NetDeltaSerialize`**:
   - 在 `FExampleArray` 结构体中重写了 `NetDeltaSerialize` 函数。
   - 调用了 `FFastArraySerializer::FastArrayDeltaSerialize` 模板方法，传入 `Items` 数组、`DeltaParms` 参数以及当前对象 `*this`。

4. **启用 `NetDeltaSerializer` 特性**:
   - 通过模板特化 `TStructOpsTypeTraits` 设置 `WithNetDeltaSerializer` 为 `true`，告诉引擎使用自定义的 `NetDeltaSerialize` 方法。

### `FastArrayDeltaSerialize` 的工作原理

`FastArrayDeltaSerialize` 是一个模板方法，专门用于处理 `TArray` 的差异化序列化。以下是它的工作流程：

1. **构建基础状态映射**:
   - 创建一个映射，关联每个元素的 `ReplicationID` 和 `ReplicationKey`。
   - 这些 ID 和 Key 用于跟踪数组中的每个元素及其状态。

2. **检测变化**:
   - 遍历当前数组中的每个元素，检查其 `ReplicationKey` 是否发生变化。
   - 如果发现新的元素或已存在的元素发生了变化，则将其标记为“脏”。

3. **生成差异包**:
   - 将新增的元素编码到网络包中。
   - 将修改过的元素编码到网络包中。
   - 将删除的元素编码到网络包中。

4. **更新基础状态**:
   - 更新基础状态映射，以反映最新的数组状态。
   - 这样，在下一次同步时，可以基于最新的基础状态来计算差异。

5. **发送网络包**:
   - 将生成的差异包发送给客户端。
   - 客户端接收到差异包后，根据其中的信息更新本地数组。

### 总结

`NetDeltaSerialize` 函数是 Unreal Engine 中用于高效网络同步的关键机制之一。通过 `FastArrayDeltaSerialize` 的实现，它可以显著减少网络流量，提高大型数组的同步性能。具体来说，它通过维护基础状态映射、检测变化、生成差异包并更新基础状态来实现这一目标。