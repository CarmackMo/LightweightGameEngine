#include "Singleton.h"

namespace Engine
{


template<class T>
inline Singleton<T>::Singleton() { }


template<class T>
inline Singleton<T>::~Singleton() { }


template<class T>
inline T* Singleton<T>::Instance()
{
	if (instance == nullptr)
		instance = new T();

	return instance;
}


template<class T>
inline void Singleton<T>::Destory()
{
	if (instance != nullptr)
		delete instance;
}


}//Namespace Engine