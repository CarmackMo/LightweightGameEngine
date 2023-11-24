/* @brief The "Singleton" class serves as a base class designed to convert any derived class
 *		  into a singleton.
 *
 *		  Ensuring that only one instance exists in the entire application is a key requirement
 *		  for a class that is also a singleton. To achieve this, the constructor of the
 *		  "Singleton" class is set to private, priventing users from directly instantiating
 *		  a class instance. Additionally, certain members such as the copy constructor and
 *		  assignment operator are explicitly deleted to prevent implicit instantiation of the
 *		  class. It is important to enforce these rules consistently in all classes derived from
 *		  the "Singleton" base class.
 *
 *		  In conclusion, to define a class as a singleton, it should be inherited from the "Singleton"
 *		  class. Optionally, the user may choose to implement the default constructor (a constructor
 *		  with no parameters) and destructor. If these functions are implemented, they should be
 *		  set to private access. It is important to note that only the default constructor should
 *		  be implemented, and other constructors should be avoided. Additionally, functions that
 *		  could implicitly create instances of the class, such as the copy constructor and assignment
 *		  operator, should be explicitly deleted. Following these guidelines will ensure the proper
 *		  implementation of a singleton class.
 *
 *		  Note that the relationship between the "Singleton" class and its derived classes remains
 *		  a regular inheritance relationship. When creating an instance of the singleton, the
 *		  constructor of the "Singleton" class is invoked first, followed by the constructors of the
 *		  derived classes. Therefore, users have the opportunity to perform initialization tasks
 *		  within the constructors.
 */

#pragma once

namespace eae6320
{

	template<class T>
	class cSingleton
	{
		// Interface
		//=========================

	public:

		inline static T* Instance()
		{
			if (instance == nullptr)
				instance = new T();

			return instance;
		}

		inline virtual void CleanUp()
		{
			if (instance != nullptr)
				delete instance;
		}


		// Implementation
		//=========================

	protected:

		inline cSingleton() = default;
		inline ~cSingleton() = default;

		inline cSingleton(const cSingleton&) = delete;
		inline cSingleton(cSingleton&&) = delete;
		inline void operator=(const cSingleton&) = delete;
		inline void operator=(cSingleton&&) = delete;


		// Data
		//=========================

	protected:

		static T* instance;

	};


	template<class T>
	T* cSingleton<T>::instance = nullptr;


}//Namespace Engine