#pragma once
#pragma once

using namespace Engine;

#pragma region Vector2

/* Constructor */
inline Vector2::Vector2() :
	x(0), y(0)
{ }
inline Vector2::Vector2(double x, double y) :
	x(x), y(y)
{ }
inline Vector2::Vector2(const Vector2& vec) :
	x(vec.x), y(vec.y)
{ }

///* Element getter */
//inline float Vector2::x(void) const { return x_; }
//inline float Vector2::y(void) const { return y_; }
///* Element setter */
//inline void Vector2::x(float x) { x_ = x; }
//inline void Vector2::y(float y) { y_ = y; }

/* Self modifying operators */
inline void Vector2::operator+= (const Vector2& vec)
{
	x += vec.x; y += vec.y;
}
inline void Vector2::operator-= (const Vector2& vec)
{
	x -= vec.x; y -= vec.y;
}

inline void Vector2::operator*= (Vector2 const& vec)
{
	x *= vec.x; y *= vec.y;
}
inline void Vector2::operator*= (double num)
{
	x *= num; y *= num;
}

inline void Vector2::operator/= (const Vector2& vec)
{
	x /= vec.x; y /= vec.y;
}
inline void Vector2::operator/= (double num)
{
	x /= num; y /= num;
}

/* Modifying operators */
inline Vector2 Vector2::operator+ (const Vector2& vec) const
{
	return Vector2(x + vec.x, y + vec.y);
}
inline Vector2 Vector2::operator- (const Vector2& vec) const
{
	return Vector2(x - vec.x, y - vec.y);
}

inline Vector2 Vector2::operator* (const Vector2& vec) const
{
	return Vector2(x * vec.x, y * vec.y);
}
inline Vector2 Vector2::operator* (double num) const
{
	return Vector2(x * num, y * num);
}
inline Vector2 operator* (double left, Vector2 right)
{
	return Vector2(left * right.x, left * right.y);
}

inline Vector2 Vector2::operator/ (const Vector2& vec) const
{
	return Vector2(x / vec.x, y / vec.y);
}
inline Vector2 Vector2::operator/ (double num) const
{
	return Vector2(x / num, y / num);
}

/* Assignment operators */
inline Vector2& Vector2::operator= (const Vector2& vec)
{
	x = vec.x; y = vec.y;
	return *this;
}

/* Comparison operators */
inline bool Vector2::operator== (const Vector2& vec) const
{
	return (x == vec.x) && (y == vec.y);
}

inline bool Vector2::operator!= (const Vector2& vec) const
{
	return (x != vec.x) || (y != vec.y);
}

/* Negate */
inline Vector2 Vector2::operator- (void) const
{
	return Vector2(-x, -y);
}

#pragma endregion



#pragma region ReferenceCount

inline ReferenceCount::ReferenceCount(unsigned long smartRef, unsigned long weakRef) :
	smartRefCount(smartRef),
	weakRefCount(weakRef)
{ }

inline ReferenceCount::~ReferenceCount()
{ }

#pragma endregion


#pragma region GameObject

inline GameObject::GameObject() :
	name(""),
	position(Vector2::Zero)
{ }

inline GameObject::GameObject(const string& name, const Vector2& position) :
	name(name),
	position(position)
{ }

inline GameObject::~GameObject()
{ }

inline SmartPtr<GameObject> GameObject::Create(const string& name, const Vector2& position)
{
	return SmartPtr<GameObject>(new GameObject(name, position));
}

#pragma endregion
