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

+ [3D图形库](#GraphicsLibrary)

+ [渲染管线](#RenderingPipeline)

+ [资产管线](#AssetPipeline)

+ [任务系统](#JobSystem)

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

为了方便理解，下面附上渲染管线的架构图：

![Rendering Pipeline Architecture](Documents/Images/RenderingPipeline. png)

+ 从渲染管线的视角来看，游戏引擎可以分为两个部分：*“应用端”* 和 *“渲染端”*。应用端和渲染端采用了 **生产者-消费者设计模式**，其中应用端作为生产者负责计算并“生产”渲染数据，而渲染端则作为消费者使用这些数据来渲染游戏画面。

+ 应用端和渲染端各自维护着一个独立的线程。应用端在 *“主线程（main thread）”* 上运行，该线程负责游戏逻辑执行和除渲染外的所有其他引擎功能。相对应地，渲染端在 *“渲染线程（rendering thread）”* 上运行，其专门负责所有与渲染相关的任务。

+ 在游戏引擎初始化时，主线程负责初始化游戏引擎所有的系统，这其中包括了渲染端。此外也是主线程负责为渲染端创建一个新的线程来作为渲染线程。     

+ 完成初始化后，主线程进入引擎的主循环（main loop）。主循环专门用于运行整体的引擎功能和游戏逻辑。主循环更新可以进一步划分为 *“系统更新”（System Update）* 和 *“模拟更新”（Simulation Update）*。
    - 模拟更新负责的是对游戏逻辑的逐帧更新，例如物理更新，摄像机更新，gameplay更新等。模拟更新的频率与游戏的帧率同步，且可以由用户自定义。
    - 另一方面，系统更新负责执行引擎系统的功能，例如监听用户输入，更新UI，向渲染端提交渲染数据等。系统更新的频率与CPU时钟周期（CPU clock cycle）同步。
    - 系统更新和模拟更新不一样的迭代频率可能会引发一些渲染问题。以渲染一个移动的模型为例，模型的变换矩阵由模拟更新负责计算，每一帧才会更新一次（大多数游戏使用30FPS或60FPS的帧率）。但系统更新会在每个CPU时钟周期向渲染端提交模型的变换矩阵进行渲染。如果系统更新直接向渲染端提交由模拟更新计算得到的变换矩阵，那么渲染得到的模型移动将会是断断续续的。

        对这类问题的解决方案是基于系统时间计算渲染数据的预测值，并将这些预测数据而不是原始数据提交给渲染端进行渲染。

    - 在m目前的实现中，需要提交的渲染数据包括了：系统时间、摄像机信息、每个游戏对象的网格、着色器和变换矩阵等。
