# 3D Game Engine


## Introduction

A lightweight game engine that is developed by C/C++ and targeted on the Windows operating system. The systems and components of this game engine include:

- **Cross-platform rendering pipeline**
- **Art asset pipeline**
- **Job system**
- **Maya plugin**
- **Utility components**
- **Math library**



<br></br>

## Catalog

+ [Graphics Library](#GraphicsLibrary)
+ [Rendering Pipeline](#RenderingPipeline)
+ [Asset Pipeline](#AssetPipeline)
+ [Job System](#JobSystem)
+ [Maya Plugin](#MayaPlugin)
+ [Utility](#Utility)
    - [Singleton](#Singleton)
    - [Smart Pointers](#SmartPointers)
+ [Math](#Math)
    - [Mathf](#Mathf)
    - [Vector](#Vector)
    - [Matrix](#Matrix)




- Designed to seamlessly run on both the x64 and Win32 platforms. Developed using Direct3D for x64 platfrom and OpenGL for Win32 platform.
- Comprehensive support for both GLSL and HLSL.
- Developed platform-specific rendering components such as "mesh" and "effect" representations. Developed platform-independent interfaces for inter-system interactions.



<br></br>
<a id="GraphicsLibrary"></a>

# 3D Graphics Library

游戏引擎的渲染管线要若要进行渲染，一个3D图形渲染库是不可缺少的。为此，该游戏引擎实现了一个简易的跨平台的3D图形渲染库。该图形渲染库具备以下特点和功能：

+ 跨平台性：该图像渲染库同时支持 x64 和 Win32 平台。在 x64 平台下该图形渲染库使用了 Direct3D 11 作为渲染后端，而在 Win32 平台下该图形渲染库使用了 OpenGL 4.6 作为渲染后端。由于在不同的平台下使用了不同的渲染后端和渲染逻辑，为此渲染库对自身的底层逻辑进行了封装，并设计了通用（universal/uniform）且平台独立（platform-independent）的接口以供外部系统的调用。该渲染库最终是静态库（Static Library）的形式部署在游戏引擎中，

+ Vertex Transform: Achieved translation, rotation, and scale by leveraging homogeneous coordinate; supported transformation between different spaces (i.e., Model Space, World Space, Image (Camera) Space, Perspective (NDC) Space, Screen Space).

+ Lighting: 基于 Lambert 光照模型实现了 Diffuse Lighting, Specular Lighting, Ambient Lighting.

+ Shading（Work in progress）: Implemented Phong shading, Gouraud shading, and Flat shading;

+ Texture（Work in progress）: Provided functionalities on applying image texture or procedure texture; performed perspective correction in affine space to correct distortion.


 
<br></br>
<a id="RenderingPipeline"></a>

# Rendering Pipeline

游戏引擎实现了一个跨平台的渲染管线。该渲染管线同时支持 x64 和 Win32 平台。在 x64 平台下该渲染管线使用了 Direct3D 11 作为渲染后端，而在 Win32 平台下该渲染管线使用了 OpenGL 4.6 作为渲染后端。由于在不同的平台下使用了不同的渲染后端和渲染逻辑，为此渲染管线对自身的底层逻辑进行了封装，并设计了通用（universal/uniform）且平台独立（platform-independent）的接口以供外部系统的调用。为了方便理解，可参考下方渲染管线的架构图

【此处插入渲染管线的架构图】

【此处是基于架构图，对渲染管线的介绍】

+ 从渲染管线的视角来看，游戏引擎可以被划分为两个部分：应用端和渲染端。两个部分各自维护着一个独立的线程。应用端的线程被称为“Game Engine Main Thread”，它负责游戏逻辑的运行，以及除了渲染以外其他所有引擎功能的运行。而渲染端的线程则被称为“Game Engine Rendering Thread”，它负责运行所有与渲染相关的功能。

+ 从渲染管线的视角来看，应用端和渲染端之间是生产者和消费者关系：应用端负责计算和“生产”渲染数据，而渲染端负责使用这些渲染数据绘制游戏画面。

+ 在游戏引擎启动时，Main Thead 负责初始化游戏引擎所有的系统，这其中包括了引擎的渲染管线。此外也是 Main Thead 负责创建一个新的线程来作为 Game Engine Rendering Thread。

+ 完成初始化后，Main Thead 会进入游戏的主循环（main loop）。主循环的功能是负责游戏每一帧的更新。主循环更新又额外分为系统更新（System Update）和模拟更新（Simulation Update）。
    - 模拟更新负责的是对游戏状态的更新，例如物理更新，摄像机更新等。模拟更新的迭代速率是与游戏的帧率绑定的。
    - 系统更新负责的是运行游戏引擎的各个系统的功能，例如监听用户输入，把渲染数据提交给渲染管线进行渲染等。系统更新的迭代速率是与 CPU clock cycle 绑定的。
    - 系统更新和模拟更新不一样的迭代速率是会引发一些渲染问题。以渲染一个移动的模型为例，由于模型的transform是在模拟更新中更新的，每一帧（通常是30FPS或60FPS）才会更新一次。而系统更新则会在每一个 CPU clock cycle 向渲染管线提交transform矩阵进行渲染。如果系统更新直接向渲染管线提交由模拟更新计算得到的transform矩阵，那么渲染得到的模型移动将会是断断续续的。对于这类问题的解决方案是，在提交渲染数据时，游戏引擎会基于系统时间计算出渲染数据的预测值，再把预测值提交给渲染管线进行渲染。
    - 在当前的实现中，渲染数据包括了 系统时间，摄像机信息，每个game object 的 mesh，shader 以及 transform矩阵 等数据。

+ 渲染数据提交至渲染端后，后续的渲染步骤由渲染管线全权负责处理。与应用端类似，渲染管线完成了自身的初始化后，会进入渲染循环（Render loop），负责游戏运行期画面的更新和渲染。如上文所述，应用端和渲染端扮演了生产者和消费者的关系，而游戏引擎使用了两个线程来分别运行应用端和渲染端。因此作为消费者的 rendering thread 需要在每一次渲染时堵塞地等待（blocking wait） main thread 完成渲染数据的提交后再进行渲染。
    - 但堵塞同步本身并不是一个很好的设计，因为这会极大地拖慢引擎的运行性能。其优点是实现相对简单，线程安全性较好且更易于维护。由于该游戏引擎的架构相对简单，因此采用了这种设计来实现应用端和渲染端的同步。

+ 渲染管线维护着两个buffer用来暂存渲染数据。其中一个是“Receiver Buffer”，它负责接收从应用端提交的渲染数据。而另一个buffer是“Render Buffer”，它负责把渲染数据从Operating System的内存中提交到GPU的内存进行渲染。在一次渲染循环中，渲染数据首先会在应用端被计算出来，然后提交至渲染端的 “Receiver Buffer” 中进行暂存。在应用端完成所有数据的提交后，渲染数据会从 “Receiver Buffer” 迁移到 “Render Buffer”。接下来 “Receiver Buffer” 会继续接收应用端提交的用于下一次渲染的渲染数据，而 “Render Buffer” 则会把该次渲染的渲染数据提交给GPU进行渲染。
    - 由于当前的渲染管线采用了阻塞等待的方式接收应用端提交的数据，因此其实只使用一个buffer来接收数据，然后提交给GPU渲染是足够的。使用双buffer的设计是考虑到未来该渲染管线会从单线程升级成多线程。一个线程负责接收应用端提交的渲染数据，另一个线程负责把渲染数据提交给GPU渲染。在多线程的设计下，如果只采用一个buffer完成上述步骤，那么正在被提交给GPU的数据很可能会被正在接收的来自应用端的数据给覆盖。因此需要采用双buffer，甚至是多个buffer的设计。

+ “Render Buffer” 完成数据提交后。渲染管线会调用渲染库的 draw call 进行该次画面的渲染。


+ ## Rendering Components


值得一提的是，渲染管线还实现了一些通用的渲染组件，例如mesh，effect，constant buffer 等。由于渲染组件所使用的图形渲染库是跨平台的，因此和图形渲染库一样，这些渲染组件的底层逻辑采用了跨平台（platform-specific）实现，并对底层逻辑进行了封装，只对外暴露了平台独立的接口以供调用。为了方便理解，以下是渲染组件的类图（class diagram）

【针对每个组件，插入组件的class diagram】

【一些简单的介绍，要避免描述得过于底层，让读者知道我实现了这几种渲染组件即可】



<br></br>
<br></br>
<a id="AssetPipeline"></a>

# Asset Pipeline

从某种角度来说，游戏引擎就像是一个库，它负责为游戏的实现和运行提供对应功能。因此对于游戏引擎来说，游戏资产（例如gameobject，mesh，shader，effect等）都属于外部数据，游戏引擎是这些数据的“消费者”。因此这些数据不应该以硬编码的形式保存在游戏引擎的代码中，而是应该是存放在电脑磁盘上，在游戏运行时再由游戏引擎从磁盘上读取并加载到游戏中。而资产管线的任务，是管理游戏资产从产生，到读取，再到加载这一系列过程。

与渲染管线相似，资产管线也是一个庞大的系统。其很多功能不是直接实现在引擎中，而是以引擎插件的形式为游戏引擎所用。而且一些游戏资产的生产源是外部的软件（比如mesh的生产源是3D建模软件Maya），资产管线也专门实现了外部软件的插件以对资产进行管理。为了方便理解，可参考下方的资产管线的架构图和流程图，（这里将以 mesh 为例）

【此处插入资产管线的架构图】

【此处是基于架构图，对资产管线的介绍】







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