#pragma once
#include <cassert>
#include <string.h>
#include <stdlib.h>

class HashedString
{
private:
	unsigned int hash;

#if defined(_DEBUG)
	char* str;
#endif


public:
	inline HashedString();
	inline ~HashedString();

	inline HashedString(const char* str);
	inline HashedString(const HashedString& other);
	inline HashedString& operator=(const HashedString& other);

	inline unsigned int Get() const;

	inline bool operator==(const HashedString& other) const;
	inline bool operator<(const HashedString& other) const;

	static inline unsigned int Hash(const char* str);
	/* @brief Calculate the hash value of the given object using FNV hash algorithm.
	 *		  See http://isthe.com/chongo/tech/comp/fnv/ */
	static inline unsigned int Hash(const void* ptr, size_t byteCount);

};
