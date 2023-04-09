# 2D Game Engine 


## Introduction

This project implements a 2D game engine that based on C/C++ and targeted on Windows platform. 


The game engine implements physics system, timer system, memory allocation system, and user input system. The memory allocation system in this game engine uses the memory allocator I developed in another [project](https://github.com/WaterFriend/MemoryAllocator) . Rendering system and user interface system will be implemented in the future version.

The game engine implements object instances monitor algorithm and runtime garbage collection algorithm using smart pointer algorithm and owner-observer development pattern. 



## 简介

这是一款基于C/C++开发的，Windows桌面平台的2D游戏引擎。  

该项目实现了游戏引擎的物理系统，计时系统，动态内存分配系统。游戏引擎的的内存分配系统使用了本人在另一个个人项目中开发的[内存分配器](https://github.com/WaterFriend/MemoryAllocator) 。该项目除了必要的Window平台库（如WIndows.h），图形渲染库（如OpenCV）和C/C++基础库（如C Runtime Library 和 C++ Standard Library）外没有依赖额外的外部库。

该引擎使用了智能指针和所有者-观察者设计模式来实现了对象实例的实时监控和运行垃圾的动态回收。

在后续的开发中将会为引擎加入渲染系统和UI系统。
