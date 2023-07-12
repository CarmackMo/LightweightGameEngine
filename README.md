# 2D Game Engine


## Introduction

This project implements a 2D game engine that based on C/C++ and targeted on Windows platform. 


The game engine implements physics system, timer system, memory allocation system, and user input system. The memory allocation system in this game engine uses the memory allocator I developed in another [project](https://github.com/WaterFriend/MemoryAllocator) . Rendering system and user interface system will be implemented in the future version.

The game engine implements object instances monitor algorithm and runtime garbage collection algorithm using smart pointer algorithm and owner-observer development pattern. 



## 简介

这是一款基于C/C++开发的，Windows桌面平台的2D游戏引擎。  

该项目实现了游戏引擎的物理系统，计时系统，动态内存分配系统。游戏引擎的的内存分配系统使用了本人在另一个个人项目中开发的[内存分配器](https://github.com/WaterFriend/MemoryAllocator) 。该项目除了必要的Window平台库（如WIndows.h），图形渲染库（如OpenCV）和C/C++基础库（如C Runtime Library 和 C++ Standard Library）外没有依赖额外的外部库。

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

The job system provides a range of APIs for users to manage tasks and the job system itself. Users can create job queues within the job system and assign tasks to specific queues based on the runtime demand of their program. Additionally, users can remove redundant job queues from the job system and remove idle job runners from specific job queues based on the workload of job system and each job queue, so as to optimize resource utilization.

Furthermore, the job system implements an automatic workload adjustment mechanism. When creating a new job queue, users can choose to apply this mechanism. The adjustment mechanism dynamically creates or removes job runners based on the number of pending jobs in each job queue it manages. This ensures an optimized balance between efficient resource utilization and task execution efficiency.

Before delving deeper into the job system, it is essential to introduce its underlying components first.

## Components

The job system is implemented using following components. Note that some of the underlying components (like `Mutex`, `Events`, `ScopeLock`, etc) are opened source for 



+ [Waitable Objects](#waitable)
+ [Hashed String](#hashedstring)
+ [Job Queue](#jobqueue)
+ [Job Runner](#jobrunner)
+ [Job System](#jobsys)



1.	任务系统是由多个任务队列组成。
2.	任务队列可被视为任务系统中任务调度的基础单位。
3.	每个任务队列都管理着多个jobrunner。Jobrunner是每个任务队列中，任务执行的基础单位。每个jobrunner维护着一个通过window api开启的线程用以执行任务。
4.	此外每个任务队列会维护着一个jobstatus实例，用以记录任务执行状态。
5.	任务系统提供了多个API供用户对任务系统进行管理。用户可以根据实际任务需求来创建新的任务队列。

一些底层部件也可以被用户用来  
 


<br></br>