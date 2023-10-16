# Total Achievement:

1. Graphic library 
2. Rndering pipeline
3. Data/File pipeline


<br></br>

# Grpahic Library

- x64 platform uses Direct3D
- Win32 platform uses OpenGL

- 图形库是 platfrom-independent 的，其接口以及底层逻辑在设计和实现之初，考虑到了支持多平台的需求。而图形库的组件，例如 mesh，shader，effect，constant buffer等，则是platform-specific的，针对 Direct3D 和 OpenGL有各自对应的实现。


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








<br></br>
<br></br>
---
分割线
---
<br></br>
<br></br>





# 2D Game Engine


## Introduction

This project implements a 2D game engine that is based on C/C++ and targeted on the Windows platform. 

The project implements the game engine's physics system, timing system, and dynamic memory allocation system. The memory allocation system of the game engine utilizes a memory allocator developed by myself in another personal project [MemoryAllocator](https://github.com/WaterFriend/MemoryAllocator). 

C++ polymorphism and C++ templates are used to implement smart pointers, enabling real-time monitoring of object references and dynamic memory reclamation at runtime.

A multi-threaded task queue system is implemented using Windows multi-thread programming, which dynamically adjusts the number of task execution threads based on the count of blocked tasks, thus optimizing the performance of the task queue execution.

In the subsequent development, collision detection system, rendering system, and UI system are queued to be developed.




## 简介

这是一款基于C/C++开发的，Windows桌面平台的2D游戏引擎。  

该项目实现了游戏引擎的物理系统，计时系统，动态内存分配系统。游戏引擎的的内存分配系统使用了本人在另一个个人项目中开发的[内存分配器](https://github.com/WaterFriend/MemoryAllocator) 。该项目除了必要的Window平台库（如WIndows.h），图形渲染库（如OpenGL）和C/C++基础库（如C Runtime Library 和 C++ Standard Library）外没有依赖额外的外部库。

+ 使用了C++多态，C++模板实现了智能指针，并以此实现了对象引用的实时监控和运行时内存的动态回收。
+ 使用Windows API 实现了多线程任务队列系统，该系统支持根据阻塞的任务数量动态增删任务执行线程，依次优化任务队列的执行性能。


在后续的开发中将会为引擎加入碰撞检测系统，渲染系统和UI系统。


## Catalog / 目录

+ [Math](#math)
    - [Mathf](#mathf)
    - [Vector](#vector)
    - [Matrix](#matrix)
+ [Utility](#utility)
    - [Singleton](#singleton)
    - [Smart Pointers](#smartptrs)
+ [Job System](#jobsys)
+ [Collision Detection](#collision)




<br></br>
<a id="math"></a>

# Math

<a id="mathf"></a>

## Mathf.h

This file implements various mathematical operations that are commonly used in calculations within the game engine. These operations are implemented as global APIs, allowing users to conveniently access them in the global scope. In current implementation, the operations primarily revolve around: value comparison, random value generation, and validation detection.

+ ### Value Comparison
    Current implementation provides 3 comparison functions with different features and a entry function:
    ```cpp
    /** Compare by checking if the absolute difference exceed threashold "maxDiff". Fastest
     *  performance but lowest accuracy. */
    bool AreEqualEps(float lhs, float rhs, float maxDiff);

    /** Compare by checking if the relative difference exceed threashold "maxDiff". More reliable
     *  than "AreEqualEps()" when inputs are small. Good balance between efficiency and accuracy. */
    bool AreEqualRel(float lhs, float rhs, float maxDiff);

    /** Compare by checking the difference of units in the last place of memory. It will first
     *  try direct comparison and abs-diff comparison, and then try Unit in the Last Place (ULPs)
     *  comparison. Lowest performance but highest accuracy. */
    bool AreEqualAccurate(float lhs, float rhs, float maxDiff, unsigned int maxULPS);

    /** General entry function of above comparison functions */
    bool AreEqual(float lhs, float rhs, float maxDiff);
    ```

    Note that the entry function `AreEqual()` utilizes macros to determine the comparison function to be used at runtime. In the current implementation, if the `USE_EFFICIENCY` macro is defined, the `AreEqualEps()` function will be invoked; if the `USE_ACCURACY` macro is defined, the `AreEqualAccurate()` function will be invoked; otherwise, the `AreEqualRel()` function will be invoked. 

+ ### Random Value Generation
    Current implementation provides 2 random value generation functions:
    ```cpp
    /** Generate a random integer value within range [lowerBound, upperBound). */
    int RandInRange(int lowerBound, int upperBound);

    /** Generate a random float value within range [lowerBound, upperBound]. */
    float RandInRange(float lowerBound, float upperBound);
    ```

    To facilitate the implementation of the two `RandInRange()` functions, a singleton class named `Mathf` is implemented. This class serves as a utility class, providing essential configurations and operations for the functions. It is important to note that `Mathf` is intended to be used internally and is "private" to users. Although users can still access the instance of `Mathf`, it is not recommended for direct usage in application development. All members within `Mathf` are designated as private, ensuring that only specific engine code has access to them.
    
    It is important to note that the range of random numbers generated by the `RandInRange()` function differs between the `int` variant and the `float` variant. This intentional design choice aligns with the behavior of Unity Engine's `Random.Range()` function. For further information and a detailed explanation of the range features in Unity's `Random.Range()`, the following blog is recommended for refereing: ["Why is Random.Range exclusive for integers but inclusive for floats"](https://answers.unity.com/questions/585266/question-about-randomrange.html)

+ ### Validation Dtection
    Functions that perform validation check for input values:
    ```cpp
    bool IsNAN(float val);
    bool IsZero(float val);
    ```


<br></br>


<a id="vector"></a>

## Vector.h

This file contains the definitions and implementations of data structures known as "vector" which are commonly used in linear algebra calculations.

+ ### Features
    - A vector is implemented by a 1-dimensional array, which can be considered as a 1xN row vector or an Nx1 column vector. The current implementation supports three types of vector classes: `Vector2`, `Vector3`, and `Vector4`, representing vectors with size of 2, 3, and 4, respectively.
    - Each vector class is implemented as a template class that only accepts numerical types (e.g., `int`, `float`, `double`, `uint8`, `uint16`...) as template arguments.
    - Additionally, each vector class provides convenient shortcuts for special vector instances, such as the `Zero` vector (0,0,0), the `Up` vector (0,0,1), the `Down` vector (0,0,-1), and so on. These instances are implemented as static constant instances. Since the compiler cannot infer the type of a static instance at compile time, these instances need to be explicitly implemented in advance. The current implementation supports `int`, `short`, `long`, `float`, and `double` types for each static instance.

+ ### Local APIs
    Each vector class offers a comprehensive set of functions and operators that are commonly used in standard calculations, which includes:
    - Type conversion
        ```cpp
        Vector2<U> ConvertTo<U>();
        ```
    - Standard operators
        ```cpp
        void operator+= (Vector<T> vec);
        void operator-= (Vector<T> vec);
        void operator*= (Vector<T> vec);
        void operator*= (T num);
        void operator/= (Vector<T> vec);
        void operator/= (T num);

        Vector<T> operator+ (Vector<T> vec);
        Vector<T> operator- (Vector<T> vec);
        Vector<T> operator* (Vector<T> vec);
        Vector<T> operator* (T num);
        Vector<T> operator/ (Vector<T> vec);
        Vector<T> operator/ (T num);

        Vector<T> operator= (Vector<T> vec);
        bool      operator== (Vector<T> vec);
	    bool      operator!= (Vector<T> vec);

        /* Negate */
        Vector<T> operator- ();
        ```
    - Indexing operators
        ```cpp
        T& operator[] (int idx);
        ```
    - Mathematical operations
        ```cpp
        float           Length();
        void            Norm();
        Vector<float>   GetNorm()
        ```

    There is a remark for the `Length()`, `Norm()`, and `GetNorm()` fuction. For vector instances with integer types, using its original type to perform intermeida calculate will cause data loss, since the result of division and square root might ba a non-integer number. In this case, one solution is to unify the return type to be `float` for integer type instances and float type instances, and let `Vector<double>` instances keep the same return type. However, the attempt to specify a different return type for `Vector<double>` instances using explicit template specialization was failed. In current implementation, all types of instances are using `float` as the only return type.

+ ### Global APIs
    Furthermore, current implementation also provides global functions specifically designed for vector calculations. Noted these global functions are defined whithin the namespace of `Vector::`.
    ```cpp
    T           Vector::Dot(Vector<T> vec1, Vector<T> vec2);            // Supports both Vector2 and Vector3
    Vector<T>   Vector::Cross(Vector<T> vec1, Vector<T> vec2);          // Supports Vector3 only
    float       Vector::Distance(Vector<T> vec1, Vector<T> vec2);       // Supports both Vector2 and Vector3
    ```


<br></br>


<a id="Matrix"></a>

## Matrix.h

This file contains the definitions and implementations of data structures known as "matrix" which are commonly used in linear algebra calculations and 3D mathematical calculations.

+ ### Features
    - The matrix class is implemented using a N-by-N 2-dimensional array. Considering that most 3D mathematical calculations involve square matrices, the matrix class only supports square matrices to simplify program structure. The current implementation includes two types of matrix classes: `Matrix3` and `Matrix4`, representing matrix with size of 3x3 and 4x4, respectively.
    - Each matrix class is implemented as a template class that only accepts numerical types (e.g., `int`, `float`, `double`, `uint8`, `uint16`...) as template arguments.
    - Additionally, each matrix class provides convenient shortcuts to generate commonly used matrix instances in mathematical calculations, such as identity matrix, rotation matrix, translation matrix, and so on. These shortcuts are implemented as static functions.

+ ### Local APIs
    Each matrix class offers a comprehensive set of functions and operators that are commonly used in linear algebra calculations and 3D mathematical calculations, which includes:
    - Type conversion
        ```cpp
        Matrix<U> ConvertTo<U>()
        ```
    - Standard operators
        ```cpp
        Matrix<T> operator+ (Matrix<T> mtx);
        Matrix<T> operator- (Matrix<T> mtx);
        Matrix<T> operator* (Matrix<T> mtx);
        Matrix<T> operator* (float val);
        Matrix<T> operator/ (float val);

        Matrix<T> operator= (Matrix<T> mtx);
        Matrix<T> operator== (Matrix<T> mtx);
        ```
    - Indexing operations
        ```cpp
        /* User can access matrix elements by using following syntex: val = Mat[row][col]
         * The first "[]" is a direct call to this operator, it returns a pointer to the starting
         * address of the specified row array. The second "[]" is a noraml indexing to the array */
        T* operator[] (int row);

        Vector<T> GetRow(int row);
        Vector<T> GetCol(int col);
        ```
    - Mathematical operations
        ```cpp
        /* Calculate the determinant of the minor matrix instead of the entire matrix */
        T Det(int row, int col);

        void        Invert();
        Matrix<U>   GetInverse<U>();

        void        Transpose();
        Matrix<T>   GetTranspose();

        Matrix<T>   ToTransform();
        Matrix<T>   GetInverseRotTrans();
        Vector<T>   TransformPoint(Vector<T> point);

        static Matrix<T> CreateIdentity();
        static Matrix<T> CreateXRotation(double rad);
        static Matrix<T> CreateYRotation(double rad);
        static Matrix<T> CreateZRotation(double rad);
        static Matrix<T> CreateTranslation(Vector<T> vec);
        static Matrix<T> CreateTranslation(T x, T y, T z);
        static Matrix<T> CreateScale(Vector<T> vec);
        static Matrix<T> CreateScale(T x, T y, T z)
        ```

+ ### Global APIs
    Furthermore, current implementation also provides global functions specifically designed for matrix calculations. In order to provide convenient access for uses, these function not included within the namespace of `Matrix`.
    ```cpp
    /* Calculate vec = Mtx * vec; (i.e. column vector) */
    Vector<T> operator* (Matrix<T> mtx, Vector<T> vec);
    /* Calculate vec = vec * Mtx; (i.e. row vector) */
    Vector<T> operator* (Vector<T> vec, Matrix<T> mtx);
    ```




<br></br>
<br></br>
<a id="utility"></a>

# Utility

<a id="singleton"></a>

## Singleton.h

+ ### Introduction

    This file implements `Singleton` class that serves as a base class designed to convert any derived class into a singleton. The `Singleton` class is implemented as a template class to ensure its wide applicability and versatility.

    Ensuring that only one instance exists in the entire application is a key requirement for a class that is also a singleton. To achieve this, the constructor of the `Singleton` class is set to private, priventing users from directly instantiating a class instance. Additionally, certain class members such as the copy constructor and assignment operator are explicitly deleted to prevent implicit instantiation of the class. It is important to enforce these rules consistently in all classes derived from the `Singleton` base class, to maintain the rule of "one instance globally".

+ ### Features
    + **Pros**
        - **Single instance**: It guarantees that only one instance of the class is created. Benefits the scenarios where only one object should exist to control actions or resources.
        - **Global access**: The singleton instance can be accessed from anywhere in the codebase.
        - **Resource sharing**: It allows multiple parts of the code to share a common resource or state, avoiding the need for redundant object creation.
    + **Cons**
        - **Global state**: Changes made to the singleton instance can affect other parts of the application, leading to potential coupling and unexpected behavior. Making the code harder to test and maintain.
        - **Thread safety**: In a multi-threaded environment, special care must be taken to ensure that the singleton is thread-safe to avoid race conditions and data inconsistencies.
        - **Dependency management**: As the singleton instance is often accessed directly instead of through interfaces or dependency injection, it will be difficult to manage dependencies and introduce coupling between classes.

+ ### Instruction

    The instruction of defining a class as a singleton shows as below:

    - Inherit the class from the `Singleton` class.
    - Optionally, the user may choose to implement the default constructor (a constructor with no parameters) and destructor. If these functions are implemented, they should be set to private access. 
    - It is important to note that only the default constructor should be implemented, and other constructors should be avoided.
    - Functions that could implicitly create instances of the class, such as the copy constructor and assignment operator, should be explicitly deleted.
    - When the application is terminated or the singleton instance is no longer needed, users should cleanup the singleton instance by manually invoking the `Destroy()` function provided by the `Singleton` class.

    Note that the relationship between the "Singleton" class and its derived classes remains a regular inheritance relationship. When creating an instance of the singleton, the constructor of the "Singleton" class is invoked first, followed by the constructors of the derived classes. Therefore, users have the opportunity to initialize members of derived classes within the constructors.

+ ### APIs
    ```cpp
    static T* Instance();
	virtual void Destory();
    ```
    

<br></br>


<a id="smartptrs"></a>

## SmartPtrs.h

This file implements smart pointers that are commonly used in dynamic memory resource management, and their necessary components. These components include `RefCount`, `PtrBase`, `SmartPtr`, and `WeakPtr`.

+ [RefCount](#refcount)
+ [PtrBase](#ptrbase)
+ [SmartPtr](#smartptr)
+ [WeakPtr](#weakptr)



<a id="refcount"></a>

+ ## RefCount

    `RefCount` serves as a control component of both `SmartPtr` and `WeakPtr`. In runtime, `RefCount` is a dynamically-allocated object that holds:
    - A pointer to the managed object;
    - A user-defined deleter;
    - The number of `SmartPtr` that own the managed object;
    - the number of `WeakPtr` that refer to the managed object.

    As a control component, `RefCount` owns the direct authority of deleting the pointer to the managed object, releasing itself, and increasing or decreasing the reference count of both `SmartPtr` and `WeakPtr`. On the other hand, `PtrBase`, `SmartPtr` and `WeakPtr` rely on the provided APIs of `RefCount` to manage this data. They are not able to directly manipulate the data themselves.

    - ### APIs
    ```cpp
    /* Delete the managed object */
    virtual void Destroy() = 0;

    /* Delete self instance */
	virtual void DeleteThis() = 0;

    void IncSmartRef();

	void IncWeakRef();

	void DecSmartRef();

	void DecWeakRef();

	unsigned long GetSmartCount();

	unsigned long GetWeakCount();
    ```


<a id="ptrbase"></a>

+ ## PtrBase

    `PtrBase` is the base class for both `SmartPtr` and `WeakPtr`. It serves as a API contract for `SmartPtr` and `WeakPtr`, defining certain rules for their member function implementations. Therefore, `PtrBase` cannot be instantiated through copy constructor or assignment operations, and its data is set to be "protected". In general, `PtrBase` specifies the implementation rules for constructors, reference increment/decrement, and data operating functions.

    In the typical implementation, `PtrBase` holds two pointers: one points to the managed object, and the other points to the control block (See "[*RefCount*](#refcount)" section for more details).

    - ### APIs
    ```cpp
    /* Constructor contract */
    /* API for standard constructor of SmartPtr */
    void StandardConstruct(T* ptr, std::function<void(T*)> deleter);

    /* API for copy constructor of SmartPtr, considering class inheritance */
    template <class U>
    void CopyConstruct(const SmartPtr<U>& other);

    /* API for move constructor of SmartPtr and WeakPtr, considering class inheritance */
    template <class U>
    void MoveConstruct(PtrBase<U>&& other);

    /* API for alias constructor of SmartPtr, considering class inheritance */
    template <class U>
    void AliasConstruct(const SmartPtr<U>& other, T* ptr);

    /* API for alias move constructor of SmartPtr, considering class inheritance */
    template <class U>
    void AliasMoveConstruct(SmartPtr<U>&& other, T* ptr);

    /* API for copy constructors of WeakPtr, considering class inheritance */
    template <class U>
	void WeakConstruct(const PtrBase<U>& other);

    /* API for copy constructor of SmartPtr that converts a WeakPtr to SmartPtr,
     * considering class inheritance */
    template <class U>
	bool ConstructFromWeak(const WeakPtr<U>& other);

    /* Reference count increment/decrement */
    void IncSmartRef();
    void DecSmartRef();
    void IncWeakRef();
    void DecWeakRef();

    /* Data operating functions */
    T*              Get();
    unsigned long   GetSmartCount();
    void            Swap(PtrBase<T>& other);
    ```


<a id="smartptr"></a>

+ ## SmartPtr

    `SmartPtr` retains shared ownership of an object through a pointer. Several `SmartPtr` objects may own the same object. The managed object is destroyed and its memory deallocated when either of the following happens:

    - The last remaining `SmartPtr` owning the object is destroyed;
    - The last remaining `SmartPtr` owning the object is assigned another pointer via `operator=` or `Reset()`.
    
    The managed object is destroyed is destroyed using delete-expression or a custom deleter that is supplied to `SmartPtr` during construction. It's important to note that `SmartPtr` does not support delete-expression for array-type objects (i.e. `delete []`) by default, it is user's responsibility to provide apporpriate deleter during construction.

    A `SmartPtr` can share ownership of an object while storing a pointer to another object. This feature allows for pointing to member objects while owning the object they belong to. The stored pointer is the one accessed by `Get()`, the dereference and the comparison operators. On the other hand, the managed pointer is the one to be destroyed when reference count reaches 0.

    A `SmartPtr` may also own no objects, in which case it is called "empty" (An empty `SmartPtr` may have a non-null stored pointer if the aliasing constructor was used to create it).

    As a derived class of `PtrBase` (See "[*PtrBase*](#ptrbase)" section for more details), `SmartPtr` holds a pointer to the managed object and a pointer to the control block. When creating a `SmartPtr` using one of its constructors, the managed object and the control block must be allocated separately. The pointer held directly by the `SmartPtr` is the one returned by `Get()`, while the pointer held by the control block is the one that will be deleted when the number of shared owners reaches 0. Note that these pointers are not necessarily equal. The current implementation of `SmartPtr` includes 4 types of constructors: standard constructor, copy constructor, alias constructor, and move constructor.
    - ### Standard Constructors:
       Standard constructors allow users to directly construct smart pointers for managing objects. Users need to provide the pointer to the object as an argument to the constructor to create smart pointer. If no argument is provided or a null pointer is passed, a default empty `SmartPtr` will be created.
       
       In the cases when the managed object has a type that cannot be deleted using the default delete-expression, users also need to provide a customized deleter.
       
       Note that constructing a new `SmartPtr` using the raw underlying pointer owned by another `SmartPtr` leads to undefined behavior. It is the responsibility of users to ensure that such situations are prevented.

    - ### Copy Constructors
        Copy constructors allow users to construct new `SmartPtr` that shares ownership of the object managed by the input `SmartPtr`. If the input `SmartPtr` manages no object, the created `SmartPtr` will be empty either. 
        
        Copy constructors support scenarios involving class inheritance, but it is the responsibility of users to ensure class `<U>` is convertible to class `<T>`.
    
    - ### Alias Constructors
        Alias constructors allow users to construct a `SmartPtr` which shares ownership information with the input `SmartPtr`, but holds an unmanaged pointer to an unrelated object. If this `SmartPtr` is the last of the group to go out of scope, it will call the stored deleter for the object originally managed by the input `SmarPtr`. However, calling `get()` on this `SmartPtr` will always return a copy of the unmanaged pointer. It is the responsibility of the users to make sure that this pointer remains valid as long as this `SmartPtr` exists. 

        Alias constructors are mainly designed for the scenarios where users need to manage the member object *B* of object *A* but do not want to explicitly manage *A*. In such cases, user can first create a `SmartPtr` to *A* using standard constructor, and then create another `SmartPtr` to *B* using alias constructor. After that, destroy the `SmartPtr` to *A*. By this approach, *A* becomes inaccessable within the scope but still alive, while *B* will remain alive as long as its `SmartPtr` exists. See ["*What is shared_ptr's aliasing constructor for*"](https://stackoverflow.com/questions/27109379/what-is-shared-ptrs-aliasing-constructor-for) for more details.

    - ### Move Constructors
        Move constructors allow users to construct a `SmartPtr` from the right reference of another `SmartPtr`. Move constructors significantly reduce the overhead of deep copying and unnecessary manipulation of reference count when creating temporary `SmartPtr` instances and initializing normal `SmartPtr` instances using those temporaries. 
        
        Temporary instances (i.e. right value) are generated in the scenarios such as returning an instance from a function or assigning the return instance of a function to a new instance. Without a defined move constructor, the compiler will invoke the copy constructor instead. However, the copy constructor typically performs deep copying, which can introduce significant overhead.
        
        See [*Introcution to move constructor*](http://c.biancheng.net/view/7847.html) for more details of move construction.

    - ### APIs
    ```cpp
    /* Standard Constructors */
    SmartPtr();
	SmartPtr(nullptr_t);
    SmartPtr(T* ptr);
    SmartPtr(T* ptr, std::function<void(T*)> deleter);

    /* Copy Constructors */
                      SmartPtr(const SmartPtr<T>& other);
    template<class U> SmartPtr(const SmartPtr<U>& other);

    /* Alias Constructors */
    template<class U> SmartPtr(const SmartPtr<U>& other, T* ptr);
	template<class U> SmartPtr(SmartPtr<U>&& other, T* ptr);

    /* Move Constructor */
                      SmartPtr(SmartPtr<T>&& other);
	template<class U> SmartPtr(SmartPtr<U>&& other);

    /* Operations */
    bool                    IsUnique();
    void                    Swap(SmartPtr<T>& other);
    void                    Reset();
    template <class U> void Reset(U* ptr);
    template <class U> void Reset(U* ptr, function<void(U*)> deleter);

    /* Accessor operators */
	T* operator->();
	T& operator*();

	/* Comparision operators */
	operator bool();
	bool operator==(std::nullptr_t);
	bool operator!=(std::nullptr_t);
	bool operator==(const SmartPtr<T>& other);
	bool operator!=(const SmartPtr<T>& other);

	/* Assignment operators */
	                  SmartPtr<T>& operator=(const SmartPtr<T>& other);
    template<class U> SmartPtr<T>& operator=(const SmartPtr<U>& other);
                      SmartPtr<T>& operator=(SmartPtr<T>&& other);
    template<class U> SmartPtr<T>& operator=(SmartPtr<U>&& other);
    ```


<a id="weakptr"></a>

+ ## WeakPtr

    `WeakPtr` is a smart pointer that holds a non-owning ("weak") reference to an object that is managed by `SmartPtr`. A `WeakPtr` must be converted to `SmartPtr` in order to access the referenced object.

    `WeakPtr` models temporary ownership: when an object needs to be accessed only if it exists, and it may be deleted at any time by someone else, `WeakPtr` is used to track the object, and it is converted to `SmartPtr` to assume temporary ownership. If the original `SmartPtr` is destroyed at this time, the object's lifetime is extended until the temporary `SmartPtr` is destroyed as well.

    Another use for `WeakPtr` is to break reference cycles formed by objects managed by `SmartPtr`. If such cycle is orphaned (i.e., there are no outside shared pointers into the cycle), the `SmartPtr` reference counts cannot reach zero and the memory is leaked. To prevent this, one of the pointers in the cycle can be made weak.

    Like `SmartPtr`, a typical implementation of `WeakPtr` stores two pointers:
    + A pointer to the control block; 
    + The stored pointer of the `SmartPtr` it was constructed from.

    A separate stored pointer is necessary to ensure that converting a `SmartPtr` to `WeakPtr` and then back works correctly, even for aliased `SmartPtrs`. It is not possible to access the stored pointer in a `WeakPtr` without converting it into a `SmartPtr`.

    - ### APIs
    ```cpp
    /* Standard Constructors */
    WeakPtr();

    /* Copy Constructors */
                       WeakPtr(const WeakPtr<T>& other);
    template <class U> WeakPtr(const WeakPtr<U>& other);
    template <class U> WeakPtr(const SmartPtr<U>& other);

    /* Move constructors */
                       WeakPtr(WeakPtr<T>&& other);
    template <class U> WeakPtr(WeakPtr<U>&& other);

    /* Operations */
    bool IsExpired() const;
    void Swap(WeakPtr<T>& other);
    void Reset();

    /* Comparision operators */
    operator bool();
    bool operator==(std::nullptr_t);
    bool operator!=(std::nullptr_t);
    bool operator==(const WeakPtr<T>& other);
    bool operator!=(const WeakPtr<T>& other);

    /* Assignment operators */
                       WeakPtr<T>& operator=(const WeakPtr<T>& other);
    template <class U> WeakPtr<T>& operator=(const WeakPtr<U>& other);
                       WeakPtr<T>& operator=(WeakPtr<T>&& other);
    template <class U> WeakPtr<T>& operator=(WeakPtr<U>&& other);
    template <class U> WeakPtr<T>& operator=(const SmartPtr<U>& other);
    ```




<br></br>
<br></br>
<a id="jobsys"></a>

# Job System

Job system is designed to utilize shared resources and multi-threads for tasks execution and managements by leveraging and encapsulating Windows APIs.

The job system provides a range of APIs for users to manage tasks and the job system itself. Users can create job queues within the job system and assign job runners to specific job queues based on the runtime demand of their program. Additionally, users can remove redundant job queues from the job system and remove idle job runners from specific job queues based on the workload of job system and each job queue, so as to optimize resource utilization.

Furthermore, the job system implements an automatic workload adjustment mechanism. When creating a new job queue, users can choose to apply this mechanism. The adjustment mechanism dynamically creates or removes job runners based on the number of pending jobs in each job queue it manages. This ensures an optimized balance between efficient resource utilization and task execution efficiency.

Before delving deeper into the job system, it is essential to introduce its underlying components first.


## Components

The job system is implemented using following components. Note that certain underlying components, such as `Mutex`, `Events`, `ScopeLock`, etc., are opened for users and can be customized by users for their specific development needs.

+ [Hashed String](#hashedstring)
+ [Waitable Objects](#waitable)
+ [Job Queue](#jobqueue)
+ [Job Runner](#jobrunner)
+ [Job System](#jobsystem)


<a id="hashedstring"></a>

+ ### HashedString.h
    A `HashedString` object stores an unsigned integer hashed value, which is generated by applying the FNV hash algorithm to a regular C-style string. In the context of the job system, the hashed string object is used as a unique identifier for each job queue, ensuring its uniqueness within the job system.  


<a id="waitable"></a>

+ ### Waitable Objects

    - #### WaitableObject.h
        This class encapsulaties Windows APIs for synchronization objects such as *Events*, *Threads*, *Mutexes*. It serves as an API contract for all objects in the game engine that are intended to be thread-waitable and synchronizable.

        Instances of this class maintain a handler to a Windows synchronization object. It is the user's responsibility to design a customized constructor and destructor to properly manage the lifecycle of the handler, ensuring proper initialization and cleanup as needed.

    - #### Event.h
        A `Event` object is a synchronization object whose state can be explicitly set to signaled by use of the *"Signal()"* function. A event object can either be a `ManualResetEvent` object or be an `AutoResetEvent` object.

        Two or more processes can create the same named event. The first process actually creates the event, and subsequent processes with sufficient access rights simply open a handle to the existing event. Note that the *"initiallySignaled"* parameter of the subsequent processes will be ignored because it have already been set by the creating process.

        The initial state of the event object is specified by the *"initiallySignaled"* parameter. All threads have the access to set or reset the event.

        When a manual-reset event object is signaled, it remains signaled until it is explicitly reset to nonsignaled by the reset function. Any number of waiting threads, or threads that subsequently begin wait operations for the specified event object, can be released while the object's state is signaled.

        When an auto-reset event object is signaled, it remains signaled until a single waiting thread is released; the system then automatically resets the state to nonsignaled. If no threads are waiting, the event object's state remains signaled.

        See [Windows Event Objects](https://learn.microsoft.com/en-us/windows/win32/sync/event-objects) for more detail.

    - #### Mutex.h
        A `Mutex` object is a synchronization object whose state is set to signaled when it is not owned by any thread, and nonsignaled when it is owned. Only one thread at a time can own a mutex object.
        
        The mutex object is useful in coordinating mutually exclusive access to a shared resource. Note that critical section objects provide synchronization similar to that provided by mutex objects, except that critical section objects can be used only by the threads of a single process.

        The creating thread of the mutex can set the *"takeOwnership"* flag to request initial ownership of the mutex. Otherwise, a thread must use the *"Acquire()"* functions to request ownership. If the mutex object is owned by another thread, the *"Acquire()"* function blocks the requesting thread until the owning thread releases the mutex object If more than one thread is waiting on a mutex, a waiting thread is selected. Do not assume a first-in, first-out (FIFO) order.

        After a thread obtains ownership of a mutex, it can specify the same mutex in repeated calls to the *"Acquire()"* function without blocking its execution. This prevents a thread from deadlocking itself while waiting for a mutex that it already owns.

        Two or more processes can create the same named mutex. The first process actually creates the mutex, and subsequent processes with sufficient access rights simply open a handle to the existing mutex.

        See [Windows Mutex Objects](https://learn.microsoft.com/en-us/windows/win32/sync/mutex-objects) for more detail.

    - #### Semaphore.h
        A `Semaphore` object is a synchronization object that maintains a count between zero and a specified maximum value. The count is decremented each time a thread completes a wait for the semaphore object and incremented each time a thread releases the semaphore. When the count reaches zero, no more threads can successfully wait for the semaphore object state to become signaled. The state of a semaphore is set to signaled when its count is greater than zero, and nonsignaled when its count is zero.

        The semaphore object is useful in controlling a shared resource that can support a limited number of users. It acts as a gate that limits the number of threads sharing the resource to a specified maximum number. If more than one thread is waiting on a semaphore, a waiting thread is selected. Do not assume a first-in, first-out (FIFO) order.

        If the name of the new semaphore object matches the name of an existing named semaphore object, the calling thread with sufficient access rights simply open a handle to the existing mutex. If the name of the new semaphore matches the name of an existing event, mutex, waitable timer, job, or file-mapping object, the creation fails. This occurs because these objects share the same namespace.

        Note that a thread that owns a mutex object can wait repeatedly for the same mutex object to become signaled without its execution becoming blocked. A thread that waits repeatedly for the same semaphore object, however, decrements the semaphore's count each time a wait operation is completed; the thread is blocked when the count gets to zero. Similarly, only the thread that owns a mutex can successfully call the mutex release function, though any thread can use semaphore release function to increase the count of a semaphore object.

        See [Windows Semaphore Ojbects](https://learn.microsoft.com/en-us/windows/win32/sync/semaphore-objects) for more detail.

    - #### ScopeLock.h
        A `ScopeLock` object is a synchronization object that maintains a pointer to a mutex object. 
        
        At the time when a scopeLock object is constructed, it attempts to acquire ownership of the associated mutex. If the mutex is currently owned by another thread, the construction of the scopeLock object will be blocked until the mutex is released. The scopeLock holds ownership of the mutex for the duration of its lifetime. Upon destruction of the scopeLock object , the mutex is automatically released.  

        The scopeLock object is useful in preventing threads from generating dead lock.


<a id="jobqueue"></a>

+ ### JobQueue.h
    
    - #### Job
        A `Job` object is a data structure that stores a specific function to be executed by one of the job runners in the job system. A job object cannot be created using copy constructor or assignment operation. Any attempt to duplicate an existing job object will result in undefined behavior.

    - #### JobStatus
        `JobStatus` serves as the control module for each job queue in the job system. A job status object records the total count of awaiting jobs and executing jobs within the job queue. Additionally, the job status object utilizes an `AutoResetEvent` objct to notify notify changes in the state of the job queue. 

        Note that each job queue is bound to a unique job status object. Therefore, a job status object cannot be created using copy constructor or assignment operation. Any attempt to duplicate an existing job status object will result in undefined behavior.

    - #### JobQueue
        A `JobQueue` object is a data structure that responsible for storing and managing jobs. The jobs within the job queue object are executed in a first-in, first-out (FIFO) order.

        The job queue object is designed to be a shared resource for job runners. To facilitate synchronization between job runners working on the same job queue, the job queue object maintains a `CONDITION_VARIABLE` object and a `CRITICAL_SECTION` object. These ensure that jobs are added to and retrieved from the queue in a synchronized manner.

        Note that due to the unique dependencies (e.g. `JobStatus`) of the job queue object, a job queue object cannot be created using copy constructor or assignment operation. Any attempt to duplicate an existing job queue object will result in undefined behavior.


<a id="jobrunner"></a>

+ ### JobRunner.h
    A `JobRunner` object is an elementary job executer of the specific job queue it assigned to. 

    The job runner object serves as the control module for the thread that performs the job execution. Each job runner object stores essential data of the thread, including the handler to the threa, thread ID, and a pointer to the associated job queue. 

    The job runner object also serves as the thread input when creating a new thread for job execution, while the instance of the job runner object is managed by the job system in the main thread.


<a id="jobsystem"></a>

+ ### JobSys.h

    - #### WorkloadManager
        `WorkloadManager` serves as a workload management module for each job queue in the job system. The workload manager object stores three flags to indicate whether the job queue should apply the automatic workload adjustment mechanism, whether the number of awaiting jobs exceeds a certain threshold, and whether the number of awaiting jobs falls below a certain threshold.

        In addition, the workload manager object holds static constants for the upper threshold, lower threshold and the interval between each workload adjustment. Current implementation doesn't provide API for users to modify these data, any changes need to be made directly in the source code.

    - #### JobQueueManager
        `JobQueueManager` serves as a comprehensive manager of a job queue. The job queue manager object oversees the essential components required for the operation of a job queue, including the `JobQueue` object, a `JobStatus` object, a list of `JobRunner` objects, and a `WorkloadManager` object.


## Architecture

To provide a clearer understanding of the job system's architecture, the following is a concise blueprint outlining its key components and their interactions.

![Architecture](Images/Docs/Architecture.png)

+ **[1]:** the job system utilizes an `unordered_map` to keep track of all job queues. Each job queue map element stores a pointer to a `JobQueueManager` object as the value, while the key type employed in the job queue map is `HashedString`, which serves as the unique identifier of each job queue.

+ **[2]:** job queue is serves as the fundamental unit for job scheduling in the job system. Each job queue manages multiple job runners, which serve as the fundamental units responsible for job execution within the job queue. 

+ **[3]:** it is important to note that each job queue must have at least one job runner during runtime. When creating a new job queue, the job system will automatically add a job runner to the queue by default. When removing job runners from a job queue, if there is only one job runner remaining in the queue, the removal will not be executed.

+ **[4]:**  the job queue object maintains a `queue` to keep track of all scheduled jobs and a flag to record the status of the job queue. These components are designed as shared resources within the job queue and are protected by a `CRITICAL_SECTION` to ensure synchronized access.

+ **[5]:** during initialization, the job system creates a default job queue by default. The default job queue is a private job queue within the job system, specifically designed to handle jobs for the functioning of the job system itself. In the current implementation, the default job queue contains a single job, which is responsible for executing the automatic workload adjustment mechanism.


## Automatic Workload Adjustment 

The number of jobs that a job queue needs to execute may vary over time. On one hand, if there are too many jobs waiting to be executed and insufficient job runners available, it can lead to a slowdown in program performance. On the other hand, if the number of job runners significantly exceeds the actual demand, those idle job runners will waste system resources. It is important to strike a balance between the demand and supply of job runners to optimize program performance and resource utilization.

The current automatic workload adjustment mechanism provides a simple approach to address the aforementioned problem. Each job queue is equipped with a `WorkloadManager` object for the workload adjustment. The `WorkloadManager` keeps track of the workload status of the associated job queue. Two flags, *"isTooMany"* and *"isTooFew"*, are used by the `WorkloadManager` to indicate the workload status: whether there are too many or too few jobs in the queue. These flags are triggered based on a comparison between the number of waiting jobs and the corresponding threshold values. The default job queue in the job system maintains a routine to continuously check the status of these flags for each WorkloadManager and dynamically adjusts the number of job runners accordingly, adding or removing them as needed.


## APIs
```cpp
/* Initialize the job system and create a default job queue */
void Init();

/* Create a new job queue with the given name and return the hashed job queue name. If a job 
 * queue with the same hashed name already exists, return the hashed name directly instead. 
 * A job queue must have at least one job runner. If the user creates a job queue with a 
 * "runnerNum" value of 0, this function will automatically create a job runner. */
HashedString CreateQueue(const string& queueName, unsigned int runnerNum = 1, bool autoFlowControl = false);

/* Add a job runner thread to the specified job queue. */
void AddRunnerToQueue(JobQueueManager* manager);

/* Add a job runner thread to the specified job queue. Return true if the job queue exists and
 * the adding is successful. Otherwise, return false. */
bool AddRunnerToQueue(const HashedString& queueName);

/* Register a job to the specified job queue. Returen true if the job queue exists and the
 * adding is successful. Otherwise, return false. */
bool AddJobToQueue(const HashedString& queueName, function<void()> jobFunction, const string& jobName = std::string());

/* Remove the first job runner from the specified job queue. The job queue must have at least
 * one job runner; otherwise, the removal will have no effect and return false. */
bool RemoveRunnerFromQueue(JobQueueManager* manager);

/* Remove the first job runner from the specified job queue. The job queue must have at least
 * one job runner; otherwise, the removal will have no effect and return false. */
bool RemoveRunnerFromQueue(const HashedString& queueName);

/* Remove the specified job queue from the job system. Return true if the job queue exists and
 * the removal is successful. Otherwise, return false. */
bool RemoveQueue(const HashedString& queueName);

/* Get the specified job queue with given queue hashed name. Return a null pointer if the job
 * queue does not exist. */
JobQueueManager* GetQueue(const HashedString& queueName);

/* Check if the specified job queue exists and has unfinished jobs. */
bool IsQueueHasJobs(const HashedString& queueName);

/* Request the job system to terminate. */
void RequestStop();

bool IsStopped();
```



 


<br></br>
<br></br>
<a id="collision"></a>

# Collision Detection

Under development...

<br></br>
