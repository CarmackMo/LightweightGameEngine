#pragma once
#include "Dependency.h"
#include "Vector.h"
#include "Vector.cpp"
#include "Matrix.h"
#include "Matrix.cpp"
#include "Util.h"
#include "Util.cpp"

using namespace std;

namespace Engine
{
/* Classes forwared declaration */
class GameObject;


//template<class T>
//class LinkedNode
//{
//public:
//	LinkedNode<T>* next;
//	LinkedNode<T>* prev;
//	T* data;
//
//	inline LinkedNode() :
//		next(nullptr),
//		prev(nullptr),
//		data(nullptr)
//	{ }
//
//	inline LinkedNode(LinkedNode<T>* next, LinkedNode<T>* prev, T* data) :
//		next(next),
//		prev(prev),
//		data(data)
//	{ }
//
//	inline ~LinkedNode()
//	{
//		delete data;
//	}
//};
//
//
//template<class T>
//class LinkedList
//{
//public:
//	int size;
//	LinkedNode<T>* head;
//	LinkedNode<T>* tail;
//
//	inline LinkedList() : 
//		size(0),
//		head(nullptr),
//		tail(nullptr)
//	{ }
//
//	inline ~LinkedList()
//	{ }
//};



class GameObject
{
private:

public:
	string name;
	Vector2<double> position;

	inline GameObject();
	inline GameObject(const string& name, const Vector2<double>& position = Vector2<double>::Zero);
	inline ~GameObject();

	inline static SmartPtr<GameObject> Create(const string& name, const Vector2<double>& position = Vector2<double>::Zero);
};


#include "DataType.inl"

}