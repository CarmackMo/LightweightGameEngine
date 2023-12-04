#pragma once

// Includes
//========

#include <Engine/Asserts/Asserts.h>

#include <functional>
#include <utility>



// Forwared Declaration
//=========================

namespace eae6320
{
	template <class T> class SmartPtr;
	template <class T> class WeakPtr;
}



// Class Declaraction
//=========================

namespace eae6320
{

	// RefCountBase Class Declaraction
	//=========================

	/* 	@brief `RefCount` serves as a control component of both `SmartPtr` and `WeakPtr`. In runtime,
	 		   `RefCount` is a dynamically-allocated object that holds:
	 				- A pointer to the managed object;
	 				- A user-defined deleter;
	 				- The number of `SmartPtr` that own the managed object;
	 				- the number of `WeakPtr` that refer to the managed object. */
	class RefCountBase
	{
		// Interface
		//=========================

	public:

		// Initialization / Clean Up
		//--------------------------

		RefCountBase() = default;
		~RefCountBase() = default;

		/* @breif Prevent instantiating RefCount by copy constructor and assignment
		 *		  operation. */
		RefCountBase(const RefCountBase&) = delete;
		RefCountBase& operator= (const RefCountBase&) = delete;

		// Reference Count Operations
		//--------------------------

		inline void IncSmartRef()
		{
			_MT_INCR(m_smartCount);
		}

		inline void IncWeakRef()
		{
			_MT_INCR(m_weakCount);
		}

		inline void DecSmartRef()
		{
			if (_MT_DECR(m_smartCount) == 0)
			{
				Destroy();
				DecWeakRef();
			}
		}

		inline void DecWeakRef()
		{
			if (_MT_DECR(m_weakCount) == 0)
			{
				DeleteThis();
			}
		}

		inline unsigned long GetSmartCount()
		{
			return m_smartCount;
		}

		inline unsigned long GetWeakCount()
		{
			return m_weakCount;
		}


		// Implementation
		//=========================

	private:

		virtual void Destroy() = 0;

		virtual void DeleteThis() = 0;


		// Data
		//=========================

	private:

		unsigned long m_smartCount = 1;
		unsigned long m_weakCount = 1;

	};


	// RefCount Class Declaraction
	//=========================

	template <class T>
	class RefCount : public RefCountBase
	{
		// Interface
		//=========================

	public:

		explicit RefCount(T* ptr, std::function<void(T*)> deleter = nullptr) :
			RefCountBase(), m_ptr(ptr), m_deleter(deleter)
		{}


		// Implementation
		//=========================

	private:

		inline void Destroy() override
		{
			if (m_deleter != nullptr)
				m_deleter(m_ptr);
			else
				delete m_ptr;
		}

		inline void DeleteThis() override
		{
			delete this;
		}


		// Data
		//=========================

	private:

		T* m_ptr;
		std::function<void(T*)> m_deleter;

	};


	// PtrBase Class Declaraction
	//=========================

	/*	@brief `PtrBase` is the base class for both `SmartPtr` and `WeakPtr`. It serves as a API
	 		   contract for `SmartPtr` and `WeakPtr`, defining certain rules for their member
	 		   function implementations. */
	template <class T>
	class PtrBase
	{
		// Interface
		//=========================

	public:

		friend class PtrBase;
		friend class SmartPtr<T>;

		PtrBase(const PtrBase<T>&) = delete;
		PtrBase& operator= (const PtrBase<T>&) = delete;

		inline unsigned long GetSmartCount() const
		{
			return m_refCount != nullptr ? m_refCount->GetSmartCount() : 0;
		}

		inline unsigned long GetWeakCount() const
		{
			return m_refCount != nullptr ? m_refCount->GetWeakCount() : 0;
		}

		// Implementation
		//=========================

	protected:

		PtrBase() = default;
		~PtrBase() = default;

		inline void StandardConstruct(T* ptr, std::function<void(T*)> deleter = nullptr)
		{
			m_ptr = ptr;
			m_refCount = new RefCount<T>(ptr, deleter);
		}

		/* @brief Performs copy-construct for SmartPtr. Using shallow copy to data, since data is
		 		  shared by SmartPtrs owning the same object. It is the responsibility of the users
		 		  to ensure "U*" is implicitly convertible to "T*" */
		template <class U>
		inline void CopyConstruct(const SmartPtr<U>& other)
		{
			other.IncSmartRef();
			m_ptr = other.m_ptr;
			m_refCount = other.m_refCount;
		}

		/* @brief Performs move-constructor for SmartPtr and WeakPtr. Using shallow copy to copy the
		 		  pointer itself. It is the responsibility of the users to ensure "U*" is implicitly
		 		  convertible to "T*" */
		template <class U>
		inline void MoveConstruct(PtrBase<U>&& other)
		{
			m_ptr = other.m_ptr;
			m_refCount = other.m_refCount;

			other.m_ptr = nullptr;
			other.m_refCount = nullptr;
		}

		/* @brief Performs aliasing-constructor for SmartPtr. It is the responsibility of the users
		 		  to ensure "U*" is implicitly convertible to "T*" */
		template <class U>
		inline void AliasConstruct(const SmartPtr<U>& other, T* ptr)
		{
			other.IncSmartRef();
			m_ptr = ptr;
			m_refCount = other.m_refCount;
		}

		/* @brief Performs aliasing-move-constructor for SmartPtr. It is the responsibility of the
		 		  users to ensure "U*" is implicitly convertible to "T*" */
		template <class U>
		inline void AliasMoveConstruct(SmartPtr<U>&& other, T* ptr)
		{
			m_ptr = other.m_ptr;
			m_refCount = other.m_refCount;

			other.m_ptr = nullptr;
			other.m_refCount = nullptr;
		}

		/* @brief Performs copy-construct for WeakPtr. Using shallow copy to data, since WeakPtr
		 		  holds non-owning reference to share data that managed by SmartPtr. It is the
		 		  responsibility of the users to ensure "U*" is implicitly convertible to "T*" */
		template <class U>
		inline void WeakConstruct(const PtrBase<U>& other)
		{
			other.IncWeakRef();
			m_ptr = other.m_ptr;
			m_refCount = other.m_refCount;
		}

		/* @brief: Performs copy-construct from WeakPtr to SmartPtr. If the given WeakPtr is empty
		 		   or is already expired, return false. Return true if construct success. */
		template <class U>
		inline bool ConstructFromWeak(const WeakPtr<U>& other)
		{
			if (other.m_refCount != nullptr && other.IsExpired() != true)
			{
				other.IncSmartRef();
				m_ptr = other.m_ptr;
				m_refCount = other.m_refCount;
				return true;
			}
			return false;
		}


		inline void IncSmartRef() const
		{
			if (m_refCount != nullptr)
				m_refCount->IncSmartRef();
		}

		inline void DecSmartRef()
		{
			if (m_refCount != nullptr)
				m_refCount->DecSmartRef();
		}

		inline void IncWeakRef() const
		{
			if (m_refCount != nullptr)
				m_refCount->IncWeakRef();
		}

		inline void DecWeakRef()
		{
			if (m_refCount != nullptr)
				m_refCount->DecWeakRef();
		}


		/* @brief Return the pointer of the managed object */
		inline T* Get() const
		{
			return m_ptr;
		}

		/* @brief Swaps the managed objects. */
		inline void SwapPtr(PtrBase<T>& other)
		{
			std::swap(m_ptr, other.m_ptr);
			std::swap(m_refCount, other.m_refCount);
		}


		// Data
		//=========================

	protected:

		T* m_ptr = nullptr;
		RefCountBase* m_refCount = nullptr;

	};


	// SmartPtr Class Declaraction
	//=========================

	/* @brief `SmartPtr` retains shared ownership of an object through a pointer. Several `SmartPtr`
	 		  objects may own the same object. The managed object is destroyed and its memory
	 		  deallocated when either of the following happens:
	 			- The last remaining `SmartPtr` owning the object is destroyed;
	 			- The last remaining `SmartPtr` owning the object is assigned another pointer via
	 			  `operator=` or `Reset()`. */
	template <class T>
	class SmartPtr : public PtrBase<T>
	{
		// Interface
		//=========================

	public:

		friend class WeakPtr<T>;

		// Initialization / Clean Up
		//--------------------------

		/* @brief Constructs a SmartPtr as the pointer to the managed object. If no input or a null
		 		  pointer is given, construct an empty SmartPtr instead. It is user's responsibility
		 		  to make sure not to constructor a SmartPtr using this constructor if the target
		 		  object is already owned by another smart pointer. */
		inline SmartPtr() { }
		
		inline SmartPtr(nullptr_t) { }
		
		inline SmartPtr(T* ptr)
		{
			this->StandardConstruct(ptr);
		}
		
		/* @brief Allow users to specify a customized deleter for data types (e.g. array type) that
		 		  cannot be deleted using regular "delete" expression. */
		inline SmartPtr(T* ptr, std::function<void(T*)> deleter)
		{
			this->StandardConstruct(ptr, deleter);
		}

		/* @brief Copy constructors. Constructs a SmartPtr which shares ownership of the object
				  managed by "other". If "other" manages no object, this instance manages no object
				  either. Using shallow copy to copy pointers only */
		inline SmartPtr(const SmartPtr<T>& other)
		{
			this->CopyConstruct(other);
		}

		template <class U>
		inline SmartPtr(const SmartPtr<U>& other)
		{
			this->CopyConstruct(other);
		}

		/* @brief Aliasing constructor. Constructs a SmarPtr which shares ownership information with
		 *		  the initial value of "other", but holds an unrelated and unmanaged pointer "ptr".
		 *		  It is the responsibility of the users to make sure that "ptr" remains valid as long
		 *		  as this SmartPtr exists */
		template <class U>
		inline SmartPtr(const SmartPtr<U>& other, T* ptr)
		{
			this->AliasConstruct(other, ptr);
		}
		
		template <class U>
		inline SmartPtr(SmartPtr<U>&& other, T* ptr)
		{
			this->AliasMoveConstruct(std::move(other), ptr);
		}

		/* @brief Move constructors. Move-constructs a SmartPtr from "other". After the construction,
		 *		  this instance contains a copy of the previous state of "other", "other" is empty and
		 *		  its stored pointer is null.  */
		inline SmartPtr(SmartPtr<T>&& other) noexcept
		{
			this->MoveConstruct(std::move(other));
		}

		template <class U>
		inline SmartPtr(SmartPtr<U>&& other)
		{
			this->MoveConstruct(std::move(other));
		}

		/* @brief Constructs a SmartPtr which shares ownership of the object managed by "other". It is
		 *		  the responsibility of the users to ensure "other" is a valid WeakPtr (i.e. "other" is
		 *		  neither empty nor the object it manages is deleted). */
		template <class U>
		inline SmartPtr(const WeakPtr<U>& other)
		{
			bool res = this->ConstructFromWeak(other);
			EAE6320_ASSERT(res == true);
		}

		inline ~SmartPtr()
		{
			this->DecSmartRef();
		}

		// Operations
		//--------------------------

		/* @brief Returns the stored pointer */
		inline T* GetPtr()
		{
			return this->m_ptr;
		}

		/* @brief Checks whether the managed object is managed only by the current SmartPtr instance. */
		inline bool IsUnique() const
		{
			return this->GetSmartCount() == 1;
		}

		/* @brief Swaps the managed objects. */
		inline void Swap(SmartPtr<T>& other)
		{
			this->SwapPtr(other);
		}

		/* @brief Release resource and convert this instance to empty SmartPtr object. */
		inline void Reset()
		{
			SmartPtr().Swap(*this);
		}
		/* @brief Release original pointer and decrement pointer's reference count. And take ownership
		 *		  of the new pointer "ptr". */
		template <class U>
		inline void Reset(U* ptr)
		{
			SmartPtr(ptr).Swap(*this);
		}
		/* @brief Release original pointer and decrement pointer's reference count. And take ownership
		 *		  of the new pointer "ptr" with deleter */
		template <class U>
		inline void Reset(U* ptr, std::function<void(U*)> deleter)
		{
			SmartPtr(ptr, deleter).Swap(*this);
		}

		// Operators
		//--------------------------

		/* Access operators */
		inline T* operator->()
		{
			return this->Get();
		}
		inline T& operator*()
		{
			return *(this->Get());
		}

		/* Comparision operators */
		inline operator bool()
		{
			return this->Get() != nullptr;
		}

		inline bool operator==(std::nullptr_t)
		{
			return this->Get() == nullptr;
		}
		inline bool operator!=(std::nullptr_t)
		{
			return this->Get() != nullptr;
		}

		inline bool operator==(const SmartPtr<T>& other)
		{
			return this->Get() == other.Get();
		}
		inline bool operator!=(const SmartPtr<T>& other)
		{
			return this->Get() != other.Get();
		}

		/* Assignment operators */
		inline SmartPtr<T>& operator=(const SmartPtr<T>& other)
		{
			SmartPtr(other).Swap(*this);
			return *this;
		}
		template <class U>
		inline SmartPtr<T>& operator=(const SmartPtr<U>& other)
		{
			SmartPtr(other).Swap(*this);
			return *this;
		}

		inline SmartPtr<T>& operator=(SmartPtr<T>&& other) noexcept
		{
			SmartPtr(std::move(other)).Swap(*this);
			return *this;
		}
		template<class U>
		inline SmartPtr<T>& operator=(SmartPtr<U>&& other)
		{
			SmartPtr(std::move(other)).Swap(*this);
			return *this;
		}
	};


	// WeakPtr Class Declaraction
	//=========================

	/* @brief WeakPtr is a smart pointer that holds a non-owning ("weak") reference to an object that is
	 		  managed by SmartPtr. It must be converted to SmartPtr in order to access the referenced
	 		  object.
	 		  WeakPtr models temporary ownership: when an object needs to be accessed only if it exists,
	 		  and it may be deleted at any time by someone else, WeakPtr is used to track the object, and
	 		  it is converted to SmartPtr to assume temporary ownership. If the original SmartPtr is
	 		  destroyed at this time, the object's lifetime is extended until the temporary SmartPtr is
	 		  destroyed as well. */
	template<class T>
	class WeakPtr : public PtrBase<T>
	{
		// Interface
		//=========================

	public:

		friend class SmartPtr<T>;

		// Initialization / Clean Up
		//--------------------------

		/* @brief Stantard constructor. Constructs an empty WeakPtr */
		inline WeakPtr() { }

		/* @brief Copy constructors. Constructs a WeakPtr which shares ownership of the object
		 		  managed by "other". If "other" manages no object, this instance manages no object
		 		  either. Using shallow copy to copy pointers. */
		inline WeakPtr(const WeakPtr<T>& other)
		{
			this->WeakConstruct(other);
		}

		template <class U>
		inline WeakPtr(const WeakPtr<U>& other)
		{
			this->WeakConstruct(other);
		}

		template <class U>
		inline WeakPtr(const SmartPtr<U>& other)
		{
			this->WeakConstruct(other);
		}

		/* @brief Move constructors. Move-constructs a WeakPtr instance from "Other". After this,
				  "other" is empty */
		inline WeakPtr(WeakPtr<T>&& other) noexcept
		{
			this->MoveConstruct(std::move(other));
		}

		template <class U>
		inline WeakPtr(WeakPtr<U>&& other)
		{
			this->MoveConstruct(std::move(other));
		}

		inline ~WeakPtr()
		{
			this->DecWeakRef();
		}

		// Operations
		//--------------------------

		/* @brief Checks whether the referenced object was already deleted */
		inline bool IsExpired() const
		{
			return this->GetSmartCount() == 0;
		}

		/* @brief Swaps the managed objects */
		inline void Swap(WeakPtr<T>& other)
		{
			this->SwapPtr(other);
		}

		/* @brief Release resource, and convert this instance to empty WeakPtr object */
		inline void Reset()
		{
			WeakPtr().Swap(*this);
		}

		// Operators
		//--------------------------

		/* Comparision operators */
		inline operator bool()
		{
			return this->Get() != nullptr;
		}

		inline bool operator==(std::nullptr_t)
		{
			return this->Get() == nullptr;
		}
		inline bool operator!=(std::nullptr_t)
		{
			return this->Get() != nullptr;
		}

		inline bool operator==(const WeakPtr<T>& other)
		{
			return this->Get() == other.Get();
		}
		inline bool operator!=(const WeakPtr<T>& other)
		{
			return this->Get() != other.Get();
		}

		/* Assignment operators */
		inline WeakPtr<T>& operator=(const WeakPtr<T>& other)
		{
			WeakPtr(other).Swap(*this);
			return *this;
		}
		template <class U>
		inline WeakPtr<T>& operator=(const WeakPtr<U>& other)
		{
			WeakPtr(other).Swap(*this);
			return *this;
		}

		inline WeakPtr<T>& operator=(WeakPtr<T>&& other) noexcept
		{
			WeakPtr(std::move(other)).Swap(*this);
			return *this;
		}
		template <class U>
		inline WeakPtr<T>& operator=(WeakPtr<U>&& other)
		{
			WeakPtr(std::move(other)).Swap(*this);
			return *this;
		}

		template <class U>
		inline WeakPtr<T>& operator=(const SmartPtr<U>& other)
		{
			WeakPtr(other).Swap(*this);
			return *this;
		}

	};

}//Namespace eae6320



// Unit test
//=========================

#include <Engine/UserOutput/UserOutput.h>

namespace eae6320
{
namespace SmartPtrTest
{

	class SmartPtrTest 
	{

	public:

		/* Preparation */
		class Member
		{
		public:
			int memVal = 1;
			Member() {}
			~Member() { eae6320::UserOutput::ConsolePrint("Destroying 'Member' class! \n"); }
		};

		class Base
		{
		public:
			int basVal = 2;
			Member* member;
			friend class Derive;

			Base() { void* temp = malloc(sizeof(Member)); member = new(temp) Member(); }
			~Base() { delete member; eae6320::UserOutput::ConsolePrint("Destroying 'Base' class! \n"); }
		};

		class Derive : public Base
		{
		public:
			int derVal = 3;
			Derive() {}
			~Derive() { eae6320::UserOutput::ConsolePrint("Destroying 'Derive' class! \n"); }
		};

		SmartPtrTest() = default;

		void SmartPtrUnitTest()
		{
			int* num = (int*)malloc(sizeof(int));
			int* arr = (int*)malloc(sizeof(int) * 10);
			std::function<void(int*)> deleter = [](int* p) { free(p); };


			/* Test for SmartPtr standard constructor and Reset() */
			SmartPtr<int> ptr0;
			SmartPtr<int> ptr1 = SmartPtr<int>(nullptr);
			SmartPtr<int> ptr2 = SmartPtr<int>(num);
			ptr2.Reset();
			SmartPtr<int> ptr3 = SmartPtr<int>(arr, deleter);
			ptr3.Reset();

			/* Test for SmartPtr alias constructor */
			void* temp = malloc(sizeof(Base));
			Base* base = new(temp) Base();
			SmartPtr<Base> ptr4 = SmartPtr<Base>(base);
			SmartPtr<Member> ptr5 = SmartPtr<Member>(ptr4, base->member);
			ptr4.Reset();
			ptr5.Reset();

			/* Test for SmartPtr copy constructor */
			temp = malloc(sizeof(Derive));
			Derive* derive = new(temp) Derive();
			SmartPtr<Derive> ptr6 = SmartPtr<Derive>(derive);
			SmartPtr<Base> ptr7 = SmartPtr<Base>(ptr6);

			/* Test for SmartPtr move constructor and move assignment */
			ptr4 = SmartPtr<Base>(ptr6);

			/* Test for comparison operators */
			bool res = ptr4 == ptr7;
			res = ptr3 == nullptr;
			res = ptr3 != nullptr;

			/* Test for Reset(T* ptr) */
			temp = malloc(sizeof(Base));
			base = new(temp) Base();
			ptr4.Reset(base);

			/* Test for accessor operators */
			ptr6->derVal = 9;
			(*ptr7).basVal = 8;
		}

		void WeakPtrUnitTest()
		{
			void* temp = malloc(sizeof(Derive));
			Derive* derive = new(temp) Derive();

			SmartPtr<Derive> sPtr1 = SmartPtr<Derive>(derive);
			SmartPtr<Base> sPtr2 = SmartPtr<Base>(sPtr1);

			/* Test for WeakPtr copy constructor */
			WeakPtr<Derive> wPtr1 = WeakPtr<Derive>(sPtr1);
			WeakPtr<Base> wPtr2 = WeakPtr<Base>(wPtr1);
			WeakPtr<Base> wPtr3 = WeakPtr<Base>(sPtr1);

			/* Test for WeakPtr default constructor and move constructor */
			WeakPtr<Base> wPtr4;
			wPtr4 = WeakPtr<Base>(wPtr1);

			/* Test for WeakPtr Rest() and assignment operators */
			wPtr4.Reset();
			wPtr4 = wPtr1;
			wPtr4 = sPtr1;	// Weak reference count should still be 5

			/* Test for creating SmartPtr from WeakPtr */
			SmartPtr<Base> sPtr3 = SmartPtr<Base>(wPtr1);

			/* Test for comparison operators */
			bool res;
			res = wPtr4 == wPtr2;
			res = wPtr4 == wPtr1;
			res = wPtr4 == nullptr;
			res = wPtr4 != nullptr;

			/* Test for Expire() */
			res = wPtr4.IsExpired();
			res = sPtr3.IsUnique();
			sPtr1.Reset();
			sPtr2.Reset();
			res = sPtr3.IsUnique();
			sPtr3.Reset();
			res = wPtr4.IsExpired();
		}

	};

}
}

