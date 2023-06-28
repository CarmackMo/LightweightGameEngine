#include <cassert>
#include <string.h>
#include <stdlib.h>
#include "HashedString.h"

namespace Engine
{
namespace JobSys
{

HashedString::HashedString() :
	hash(Hash(""))
{ }


HashedString::HashedString(const char* str) :
	hash(Hash(str))
{
#if defined(_DEBUG)
	this->str = _strdup(str);
#endif
}


HashedString::HashedString(const HashedString& other) :
	hash(other.hash)
{
#if defined(_DEBUG)
	this->str = _strdup(other.str);
#endif
}


HashedString::~HashedString()
{
#if defined(_DEBUG)
	if (str)
		free(str);
#endif
}


HashedString& HashedString::operator=(const HashedString& other)
{
	this->hash = other.hash;

#if defined(_DEBUG)
	if (str)
		free(str);
	this->str = other.str;
#endif

	return *this;
}


unsigned int HashedString::Get(void) const
{
	return hash;
}


bool HashedString::operator==(const HashedString& other) const
{
	return this->hash == other.hash;
}


bool HashedString::operator<(const HashedString& other) const
{
	return this->hash < other.hash;
}


unsigned int HashedString::Hash(const char* str)
{
	assert(str);

	const void* ptr = reinterpret_cast<void*>(const_cast<char*>(str));
	register const unsigned char* p = static_cast<const unsigned char*>(ptr);
	size_t byteCount = strlen(str);
	unsigned int hash = 2166136261;

	for (size_t i = 0; i < byteCount; ++i)
		hash = 16777619 * (hash ^ p[i]);

	return hash ^ (hash >> 16);
}


}//Namespace Engine
}//Namespace JobSys
