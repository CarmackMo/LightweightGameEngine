# 3D 游戏引擎

## 介绍

A lightweight game engine that is developed by C/C++ and targeted on the Windows operating system. The systems and components of this game engine include:

- **Cross-platform rendering pipeline**
- **Art asset pipeline**
- **Job system**
- **Maya plugin**
- **Utility components**
- **Math library**

英文版项目介绍可见此链接:
[English](README.md)



<br></br>

## 目录

+ [渲染管线](#RenderingPipeline)

+ [资产管线](#AssetPipeline)

+ [任务队列系统](#JobSystem)

+ [Maya插件](#MayaPlugin)

+ [通用组件](#Utility)
    - [单例](#Singleton)
    - [智能指针](#SmartPointers)

+ [数学库](#Math)
    - [Mathf](#Mathf)
    - [向量](#Vector)
    - [矩阵](#Matrix)

+ [未来计划](#FuturePlan)





<br></br>
<a id="RenderingPipeline"></a>

# 渲染管线

游戏引擎实现了一个跨平台的渲染管线。该渲染管线同时支持 **x64** 和 **Win32** 平台。在x64平台下渲染管线使用了 **Direct3D 12** 作为渲染后端；而在 Win32 平台下，渲染管线使用了 **OpenGL 4.6** 作为渲染后端。
由于在不同平台上使用了不同的渲染后端和逻辑，为此渲染管线对其底层逻辑进行了封装，并为外部系统的调用提供了通用、独立于平台的接口。


## 架构设计

为了方便理解，下面附上渲染管线的架构图：

![Rendering Pipeline Architecture](Documents/Images/RenderingPipeline.png)

+ 从渲染管线的视角来看，游戏引擎可以分为两个部分：*“应用端”* 和 *“渲染端”*。应用端和渲染端采用了 **生产者-消费者设计模式（Producer-Consumer Design Pattern）**，其中应用端作为生产者负责计算并“生产”渲染数据，而渲染端则作为消费者使用这些数据来渲染游戏画面。

+ 应用端和渲染端各自维护着一个独立的线程。应用端在 *“主线程（main thread）”* 上运行，该线程负责游戏逻辑执行和除渲染外的所有其他引擎功能。相对应地，渲染端在 *“渲染线程（rendering thread）”* 上运行，其专门负责所有与渲染相关的任务。

+ 在游戏引擎初始化时，主线程负责初始化游戏引擎所有的系统，这其中包括了渲染端。此外也是主线程负责为渲染端创建一个新的线程来作为渲染线程。     

+ 完成初始化后，主线程进入引擎的主循环（main loop）。主循环专门用于运行整体的引擎功能和游戏逻辑。主循环更新可以进一步划分为 *“系统更新”（System Update）* 和 *“模拟更新”（Simulation Update）*。
    - 模拟更新负责的是对游戏逻辑的逐帧更新，例如物理更新，摄像机更新，gameplay更新等。模拟更新的频率与游戏的帧率同步，且可以由用户自定义。
    - 另一方面，系统更新负责执行引擎系统的功能，例如监听用户输入，更新UI，向渲染端提交渲染数据等。系统更新的频率与CPU时钟周期（CPU clock cycle）同步。
    - 系统更新和模拟更新不一样的迭代频率可能会引发一些渲染问题。以渲染一个移动的模型为例，模型的变换矩阵由模拟更新负责计算，每一帧才会更新一次（大多数游戏使用30FPS或60FPS的帧率）。但系统更新会在每个CPU时钟周期向渲染端提交模型的变换矩阵进行渲染。如果系统更新直接向渲染端提交由模拟更新计算得到的变换矩阵，那么渲染得到的模型移动将会是断断续续的。

        对这类问题的解决方案是基于系统时间计算渲染数据的预测值，并将这些预测数据而不是原始数据提交给渲染端进行渲染。

    - 在目前的实现中，需要提交的渲染数据包括了：系统时间、摄像机信息、每个游戏对象的网格、着色器和变换矩阵等。

+ 渲染数据提交至渲染端后，后续的渲染步骤由渲染端负责处理。与应用端类似，渲染端完成了自身的初始化后会进入渲染循环（Render loop），负责游戏运行期的画面渲染。如前所述，应用端和渲染端分别充当了渲染数据的“生产者”和“消费者”，而游戏引擎使用了两个不同的线程来运行应用端和渲染端，致使应用端和渲染端之间存在线程同步问题。为此，作为数据消费者的渲染线程会在每一个渲染周期阻塞地等待应用端完成渲染数据的提交后，再对数据进行渲染。
    - 但是，使用阻塞机制进行同步并不是一个很好的设计，因为这会极大地拖慢引擎的运行性能。然而其优点在于实现相对简单，线程安全性较好且更易于维护。考虑到该游戏引擎的架构相对简单，因此采用了这种设计来实现应用端和渲染端的同步。

+ 渲染端维护着两个用来暂存渲染数据的缓冲区，分别是：*“接收缓冲区（Receiver Buffer）”* 和 *“渲染缓冲区（Render Buffer）”*。接收缓冲区负责接收应用端提交的渲染数据，而渲染缓冲区则将渲染数据从操作系统的内存中提交到GPU的内存进行渲染。在一个渲染周期中，应用端计算好的渲染数据会首先被提交到接收缓冲区进行暂存。在应用端完成所有数据的提交后，渲染管线会把渲染数据从接收缓冲区迁移到渲染缓冲区。完成数据转移后，接收缓冲区会继续接收应用端提交的用于下一个周期的渲染数据，而渲染缓冲区则会把当前周期的渲染数据提交给GPU进行渲染。
    - 考虑到当前的渲染管线采用阻塞等待的方法接收数据，单一缓冲区就足以从应用端接收并随后提交数据给GPU。而选择使用双缓冲区的设计是考虑到在未来的更新中，届时渲染管线可能从单线程转为多线程。对于多线程渲染管线，若只采用一个缓冲区完成上述步骤，那么正在被提交给GPU的数据很可能会被应用端提交的新数据给覆盖，因此需要双重或甚至多重缓冲区的设计。
    
+ 在渲染缓冲区完成数据提交后，渲染管线最后调用图形库的绘制命令进行画面的渲染。


+ ## 渲染组件

    为了支持渲染管线的运作，该游戏引擎实现了渲染数据类，例如网格类（mesh class），效果类（effect class），常缓冲区类（constant buffer class）。由于渲染管线是跨平台的，因此这些渲染组件的底层逻辑也采用了跨平台实现并进行了封装，只对外暴露了平台独立（platform-independent）的接口以供调用。

    此外，如前所述渲染管线采用了 “生产者-消费者” 设计模式，因而渲染数据在渲染管线的运行过程中涉及了大量的迁移。为了更好地管理渲染数据的创建、拷贝、迁移和清理，渲染数据使用了 **观察者设计模式（Observer Design Pattern）** 进行开发。




<br></br>
<br></br>
<a id="AssetPipeline"></a>

# 资产管线

从某种角度来说，游戏引擎可以理解为负责为游戏的开发和运行提供所需的特定功能的库。因此对于游戏引擎来说，游戏资产（例如gameobject，mesh，shader，user data等）都属于外部数据。游戏引擎充当这些数据的“消费者”。因此，这些数据不应硬编码到游戏引擎的源代码中。相反，它们应存储在磁盘上并在运行时加载到游戏中。资产管线的主要作用是监督和管理游戏资产从创建、到存储、再到加载到游戏中的整个过程。

与渲染管线一样，资产管线也构成了一个复杂的系统。但资产管线比较特殊的地方在于，其许多功能不会直接作用与游戏中。此外，一些游戏资产来源于外部软件（例如，模型资产来自3D建模软件 **Maya**）。因此，资产管线也专门实现了外部软件的插件以对资产进行管理。


## 架构设计

为了方便理解，以下部分以资产管线对模型资产的管理为例，展示了资产管线的架构和流程。

![Asset Pipeline Architecture](Documents/Images/AssetPipeline.png)


+ 当前的游戏引擎使用由Maya生产的模型。由于Maya对于游戏引擎来说属于外部的第三方软件，因此资产管线需要使用插件来从Maya中提取模型数据。Autodesk，Maya的母公司，为开发人员提供了SDK和API，如 **Maya Devkit** 和 **OpenMaya**。当前的游戏引擎使用了这些SDK实现了一个能够从Maya中提取模型数据的插件。游戏引擎会在构建期（build-time）对插件进行编译构建，然后由用户载入到Maya中。

+ 当游戏美术（game artist）和技术美术（technical artist）完成对模型的创建和修改后。用户可以使用插件导出模型数据。在当前的实现中，插件会从Maya中导出模型的顶点位置（vertex position），法向量（normal），切线值（tangent），顶点颜色（vertex color），以及顶点索引的构成顺序（vertex index winding order）。最后插件会把Maya模型导出为 .lua 格式的模型文件。
    - 为何将模型导出为.lua文件？虽然 .lua 作为一种人类可读的文件格式（human-readable file format），在序列化/反序列化效率和安全性方面有局限性，但其可读性良好，且易于开发人员进行维护和调试。
    
        以模型资产为例。在游戏开发过程中，包括游戏美术、技术美术和游戏工程师在内的各种职位将合作开发模型。在这个阶段，数据的可读性、可维护性和可调试性是至关重要的，因此采用人类可读文件格式来保存数据更为合适。当开发阶段结束，数据已经定型后，重点就转向数据的易处理性，易保存性和安全性。在这个阶段，使用二进制文件格式存储数据是最佳的。

+ 完成Maya模型的导出后，模型文件会被存放在电脑磁盘中，或是版本管理系统中（source control）。在游戏的构建期（build-time），游戏引擎专门用于处理模型数据的模块 *“MeshBuilder”* 将从磁盘上加载 .lua 模型文件，并对模型数据进行预处理。
    -  如：Maya模型的顶点索引构成顺序（vertex index winding order）默认采用了右手系顺序。由于游戏引擎的渲染管线在x64平台支持Direct3D，在Win32平台支持OpenGL，Direct3D使用左手系顺序。因此 MeshBuilder 会在预处理时把输出至 x64 平台的模型资产的索引顺序改为左手系顺序。

+ 在预处理阶段之后，MeshBuilder 会把模型数据编译为二进制数据，并导出为 .mesh 格式的二进制模型文件。
    - 注：.mesh 是一个自定义的文件扩展名。采用 .mesh 作为模型文件拓展名是为了方便用户对多种游戏资产文件进行管理。用户可以根据自身需求使用其他名字作为模型文件的拓展名。




<br></br>
<br></br>
<a id="JobSystem"></a>

# 任务队列系统

任务队列系统设计为使用多线程技术来管理游戏引擎各项任务的执行。

任务队列系统为用户提供了一系列API，以便管理任务的执行和任务队列系统本身的运行。用户可以以根据运行时的需求动态地增加任务队列（Job Queue）并指派任务执行单元（Job Runner），从而提高游戏引擎的性能。此外，用户可以根据任务队列系统的工作负载删除多余的任务队列和空闲的任务执行单元，以优化游戏引擎的资源利用。

此外，任务队列系统实现了一个自动化工作负载调整机制。当创建新的任务队列时，用户可以选择是否在任务队列上应用此机制。该机制根据任务队列中待处理任务的数量动态地创建或删除任务执行单元。从而确保了资源利用和任务执行效率之间的最佳平衡。

在进一步介绍任务队列系统之前，首先介绍需要其基础组件。


## 构成组件

任务队列系统的实现使用了以下组件。其中一些组件除了在任务队列系统，也可以应用在其他开发场景中，如 `互斥锁（Mutex）`、`局域锁（ScopeLock）`、`事件（Event）` 等。用户可根据特定的开发需求使用。

+ [哈希字符串（Hashed String）](#HashedString)
+ [可等待对象（Waitable Objects）](#Waitable)
+ [任务对象（Job）](#Job)
+ [任务队列（Job Queue）](#JobQueue)
+ [任务执行单元（Job Runner）](#JobRunner)
+ [任务队列管理器（Job Queue Manager）](#JobQueueManager)
+ [工作负载管理器（Workload Manager）](#WorkloadManager)


<a id="HashedString"></a>

+ ### 哈希字符串（Hashed String）
    哈希字符串对象存储了一个整型哈希值，该值是采用 **FNV哈希算法** 对字符串计算得出的。在任务队列系统的上下文中，哈希字符串对象用作每个任务队列的唯一标识符，确保其在任务队列系统中的唯一性。





<a id="Waitable"></a>

+ ### Waitable Objects

    - #### Event
        A `Event` object is a synchronization object whose state can be explicitly set to signaled by use of the *"Signal()"* function. A event object can either be a `ManualResetEvent` object or be an `AutoResetEvent` object.

        When a manual-reset event object is signaled, it remains signaled until it is explicitly reset to nonsignaled by the reset function. Any number of waiting threads, or threads that subsequently begin wait operations for the specified event object, can be released while the object's state is signaled.

        When an auto-reset event object is signaled, it remains signaled until a single waiting thread is released; the system then automatically resets the state to nonsignaled. If no threads are waiting, the event object's state remains signaled.

        See [Windows Event Objects](https://learn.microsoft.com/en-us/windows/win32/sync/event-objects) for more detail.

    - #### Mutex
        A `Mutex` object is a synchronization object whose state is set to signaled when it is not owned by any thread, and nonsignaled when it is owned. Only one thread at a time can own a mutex object.
        
        The mutex object is useful in coordinating mutually exclusive access to a shared resource. Note that critical section objects provide synchronization similar to that provided by mutex objects, except that critical section objects can be used only by the threads of a single process.

        See [Windows Mutex Objects](https://learn.microsoft.com/en-us/windows/win32/sync/mutex-objects) for more detail.

    - #### Semaphore
        A `Semaphore` object is a synchronization object that maintains a count between zero and a specified maximum value. The count is decremented each time a thread completes a wait for the semaphore object and incremented each time a thread releases the semaphore. When the count reaches zero, no more threads can successfully wait for the semaphore object state to become signaled. The state of a semaphore is set to signaled when its count is greater than zero, and nonsignaled when its count is zero.

        The semaphore object is useful in controlling a shared resource that can support a limited number of users. It acts as a gate that limits the number of threads sharing the resource to a specified maximum number. If more than one thread is waiting on a semaphore, a waiting thread is selected. Do not assume a first-in, first-out (FIFO) order.

        See [Windows Semaphore Ojbects](https://learn.microsoft.com/en-us/windows/win32/sync/semaphore-objects) for more detail.

    - #### ScopeLock
        A `ScopeLock` object is a synchronization object that maintains a pointer to a mutex object. 
        
        At the time when a scopeLock object is constructed, it attempts to acquire ownership of the associated mutex. If the mutex is currently owned by another thread, the construction of the scopeLock object will be blocked until the mutex is released. The scopeLock holds ownership of the mutex for the duration of its lifetime. Upon destruction of the scopeLock object , the mutex is automatically released.  

        The scopeLock object is useful in preventing threads from generating dead lock.













<br></br>
<br></br>
<a id="FuturePlan"></a>

# 未来计划

+ ## 渲染管线

    1. **多线程优化:** 计划采取有着更优运行性能的线程同步策略，以简化渲染过程并确保线程之间更好的同步。

    2. **扩展平台和后端支持:** 计划令渲染管线支持更多的平台，例如 Linux, MacOS, 和移动端平台等。此外，也有计划扩展3D图形库的支持，如增加对Vulkan和Metal的支持。

    3. **后处理效果:** 有计划整合各种后处理效果以提高视觉美感。例如色调映射（tone mapping）、模糊（blurring）和屏幕空间环境遮挡（Screen Space Ambient Occlusion）等。

    4. **基于物理的渲染（PBR）:** 有意向整合与物理相关的渲染技术。如实时阴影（real-time shadows）、全局光照（global illumination）等。


+ ## 资产管线

    1. **自动化：** 当前资产管线中的许多过程都依赖于用户的手动干预。例如Maya插件的安装。为了改进这一点，计划在未来的开发中使资产管线的运作更加自动化。

    2. **资产编辑器：** 在当前的实现下，对资产文件的任何编辑都需要用户直接操作源代码。这个过程操作难度大，而且容易出错，特别是对于那些不熟悉基于代码的操作的人。计划在未来开发一个提供图形用户界面(GUI)的资产编辑器。

    3. **错误检测机制：** 当前资产管线对数据的读取和处理采用了较为单一的错误检测机制。虽然它解决了基本的错误场景，但在全面性方面还有提高的空间。未来的开发打算引入一个更为稳健和全面的错误检测机制。