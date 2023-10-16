# Total Achievement:

1. Graphic library 
2. Rndering pipeline
3. Data/File pipeline


<br></br>

# Grpahic Library

- x64 platform uses Direct3D
- Win32 platform uses OpenGL

- 图形库是 corss-platform 的，其接口以及底层逻辑在设计和实现之初，考虑到了支持多平台的需求。而图形库的组件，例如 mesh，shader，effect，constant buffer等，则是platform-specific的，针对 Direct3D 和 OpenGL有各自对应的实现。


```
TODO: 
1. 需要更详细得解释 system update 和 simulation update 的区别，simulation 和 non-simulation 的逻辑。
2. 需要解释为何渲染管线不属于simulation的一部分（大智对游戏引擎不熟悉），mian thread 和 rendering thead 是生产者和消费者的关系吗？会有asynchronize的问题吗？
3. 讲得太底层（比如说下面说到 “主线程就是一个while(true)循环”），给人的感觉这个README像是写给自己的看的，而不是放在简历上用来对外展示自己skillset的，面向对象是自己，而不是面试官。容易让人underestimate我的工作。
4. 可以考虑放 rendering pipeline 的架构图和流程图。
5. 展示我用到的skill set，以及我的工作的能做什么，做到了什么程度，我这个项目的架构是怎么设计的。考虑到可能有的人会想clone下来跑，最好加上 how to run 的教程
```

<br></br>

# Rendering Pipeline

- 为了更好的介绍渲染管线，需要先介绍游戏引擎的运行架构。游戏引擎分为两个线程
    1. 主线程 （mian thread）
    2. 渲染线程 （rendering thread）


- 主线程本质上就是一个 `while(true)` 循环。主线程中的循环负责两种更新（update）：
    1. system update: 该update就是 `while(true)` 循环本身，负责执行一些引擎系统的底层逻辑（non-simulation），速率与 `while（true）`循环的速率一致，也就是 cpu tick 频率。

    2. simulation update：该 update 也在 system update 所处的 `while(true)` 循环中执行，但执行速率受用户设置的 simulation rate 限制。simulation update 用于模拟游戏引擎的帧率更新，游戏引擎的所有游戏逻辑都应该在 simulation update 中运行。

- 渲染线程本质上也是一个 `while(true)` 循环。渲染线程只负责向 GPU 提交渲染数据 （例如 vertex，index，shader等）然后调用渲染库接口 （Direct3D 和 OpenGL）进行绘制。




- 渲染管线的流程如下：
    1. 引擎的主线程进行初始化，包括对引擎的各个系统进行初始化，对渲染线程进行初始化，以及对游戏的各项资源（例如camera，各个gameobject，gameobject各自的 mesh，effect，rigid body等数据）进行初始化。

    2. 主线程会负责在 simulation update 中，也就是每一帧中，更新游戏中的各项数据，比如更新 gameobject 的位置，速度等数据。
        - 注：由于数据更新是每一帧更新一次，对于渲染来说，直接使用gameobject的顶点位置来渲染会导致gameobject在游戏画面中的移动会一卡一卡的。因此 simulation update 的职责是只负责更新数据本身。但渲染的时候不能直接使用 simulation update 的数据进行渲染。

    3. 此外主线程会负责在 system update 中，也就是每一个 CPU tick 中，向渲染线程同时也是向渲染库提交需要渲染的数据。在此之后，渲染流程就全权交由渲染线程处理。

    4. 到了渲染库这一步处理后，渲染库维持着两个buffer，一个buffer叫 `s_dataBeingSubmittedByApplicationThread`，他负责接收从主线程提交的渲染数据，另一个buffer叫 `s_dataBeingRenderedByRenderThread`，渲染库将会把这个buffer中数据提交给GPU进行渲染。

    5. 在渲染线程中，在每一个循环中，渲染线程会阻塞地等待着下一帧需要渲染的数据从主线程提交到 `s_dataBeingSubmittedByApplicationThread` buffer中。当提交完成后，渲染线程会把渲染数据从 `s_dataBeingSubmittedByApplicationThread` buffer 拷贝到 `s_dataBeingRenderedByRenderThread` buffer。然后调用渲染库API把数据从 `s_dataBeingRenderedByRenderThread` buffer 提交到GPU，并通知GPU渲染下一帧的画面。

    
<br></br>

# Data/File Pipeline

对于游戏引擎来说，gameobject的transform，mesh，shader，effect等都属于用户自定义数据（这里的用户指的是gameplay programmer 和 graphic programmer而不是游戏玩家）。因此这些数据不应该以硬编码的形式写在游戏引擎的代码中，而是应该是存放在电脑磁盘上，在游戏运行时再从磁盘上读取加载到游戏中。

数据管线的任务是负责管理这些数据从源头，到载入到游戏的整个流程。当前项目只实现了针对渲染资产（rendering asset，即mesh和shader）的数据管线，针对gameobject的数据管线将在后期开发中实现。
当前引擎的数据管线能够从3D建模软件Maya中导出模型的网格数据，并以Lua文件的形式保存在电脑的磁盘上，在编译构建游戏的时候，数据管线会读取之前提取好的Lua模型文件，并在构建时（build-time）把Lua格式的模型文件编译成二进制文件，与其他游戏数据文件存放在一起。最后在游戏运行时（run-time）由游戏程序直接对二进制模型文件进行读取与加载。

- 数据管线的流程：
    1. 渲染资产的源头（source）是建模软件Maya。Maya对于我的游戏引擎来说属于第三方软件，因此游戏引擎根据Maya提供的API和SDK实现了一个Maya插件，用以从Maya中导出模型数据。在目前的实现中，插件会从Maya中导出模型的顶点位置，法向量（normal），切线值（tangent），顶点颜色，以及顶点三角形的构成顺序（winding order）。并导出成Lua文件。
        - 注：为何要导出为Lua文件？在插件端直接就导出成二进制文件固然是可以的，但对于用户来说，也就是这个引擎的使用者来说，他们仍然需要对导出的数据文件进行二次开发工作。一个在游戏开发中比较常见的场景是：3D建模师完成建模任务后会使用插件对模型进行导出，然后技术美术和游戏开发工程师会对模型进行各项后期处理（比如骨绑 rigging）。在这个阶段中，数据文件仍未完成其所有的开发流程，数据在这个阶段需要强调其可读性和可纠错性，因此数据需要以人类可读的格式（human-readable format）保存，例如Lua或JSON。在数据走完所有开发流程，已经定型后，这个时候强调的则是数据的易处理性，易保存性以及安全性，这个时候才应该把数据编译成二进制文件。
    
    2. 在引擎对游戏进行构建时（build-time）。引擎的 `MeshBuilder` 会读取已经导出的Lua格式的模型的，并把模型数据编译成二进制文件（为了方便与Lua格式的模型文件区分开来，二进制格式的模型文件的拓展名被定义为.mesh，但具体是可以由用户决定的）。在构建这一步中，`MeshBuilder` 也会根据不同的平台对模型数据进行相应的修饰，例如Maya的三角形渲染顺序采用的是右手系，在针对Direct3D进行构建的时候，`MeshBuilder` 会先把顶点索引信息（index winding order）调整为左手系顺序，再编译成二进制文件。

    3. 在游戏运行时（run-time），在主线程对游戏中的各个资源进行加载和初始化时，二进制格式的模型数据会被解析，并用以初始化其对应的 gameobject 。