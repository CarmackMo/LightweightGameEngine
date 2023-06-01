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


## Catalog

+ [Math](#math)
    - [Mathf](#mathf)
    - [Vector](#vector)





---
<a id="math"></a>

# Math

<a id="mathf"></a>

## Mathf.h

This file implements various mathematical operations that are commonly used in calculations within the game engine. These operations are implemented as global APIs, allowing users to conveniently access them in the global scope. In current implementation, the operations primarily revolve around: value comparison, random value generation, and validation detection.

+ ### Value Comparison
    Current implementation provides 3 comparison functions with different features and a entry function:
    ```cpp
    /** Compare by checking if the absolute difference exceed threashold "maxDiff" 
     *  Fastest performance but lowest accuracy. */
    bool AreEqualEps(float lhs, float rhs, float maxDiff);

    
    bool AreEqualRel(float lhs, float rhs, float maxDiff);
    bool AreEqualAccurate(float lhs, float rhs, float maxDiff, unsigned int maxULPS);
    bool AreEqual(float lhs, float rhs, float maxDiff);
    ```




<a id="vector"></a>

## Vector.h

This file contains the definitions and implementations of data structures known as "vector" which are commonly used in linear algebra calculations.

+ ### Features
    - A vector is implemented by a one-dimensional array, which can be considered as a 1xN row vector or an Nx1 column vector. The current implementation supports three types of vector classes: `Vector2`, `Vector3`, and `Vector4`, representing vectors with size of 2, 3, and 4, respectively.
    - Each vector class is implemented as a template class that only accepts numerical types (e.g., `int`, `float`, `double`, `uint8`, `uint16`...) as template arguments.
    - Additionally, each vector class provides convenient shortcuts for special vector instances, such as the `Zero` vector (0,0,0), the `Up` vector (0,0,1), the `Down` vector (0,0,-1), and so on. These instances are implemented as static constant instances. Since the compiler cannot infer the type of a static instance at compile time, these instances need to be explicitly implemented in advance. The current implementation supports `int`, `short`, `long`, `float`, and `double` types for each static instance.

+ ### Local APIs
    Each vector class offers a comprehensive set of functions and operators that are commonly used in standard calculations, which includes:
    - Type conversion
    - Numerical operators
    - Assignment operators
    - Comparison operators
    - Indexing operators

    There is a remark for the `Length()`, `Norm()`, and `GetNorm()` method. For vector instances with integer types, using its original type to perform intermeida calculate will cause data loss. Since the result of division and square root might ba a non-integer number. In this case, one solution is to unify the return type to be `float` for integer type instances and float type instances, and let `Vector<double>` instances keep the same return type. However, the attempt to specify a different return type for `Vector<double>` instances using explicit template specialization was failed. In current implementation, all types of instances are using `float` as the only return type.

+ ### Global APIs
    Furthermore, current implementation also provides global functions specifically designed for vector calculations. Noted these global functions are defined whithin the namespace of `Vector::`.
    ```cpp
    T           Vector::Dot(vec1, vec2);            // Supports both Vector2 and Vector3
    Vector<T>   Vector::Cross(vec1, vec2);          // Supports Vector3 only
    float       Vector::Distance(vec1, vec2);       // Supports both Vector2 and Vector3
    ```


