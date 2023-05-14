#pragma once

namespace Engine
{

/**
* @brief TODO: singleton...
*
* @remarks: To prevent direct construction of a singleton class, the singleton's
*			 constructor is set to be private. Also, functions that we don't want
*			 (like: copy constructor, assignment operator) is explicitly deleted.
*/
template<class T>
class Singleton
{
protected:
	static T* instance;

	inline Singleton();
	inline ~Singleton();

public:
	inline Singleton(const Singleton& other) = delete;
	inline void operator=(const Singleton& other) = delete;


	inline static T* Instance();

	inline virtual void Destory();
};

template<class T>
T* Singleton<T>::instance = nullptr;


}//Namespace Engine