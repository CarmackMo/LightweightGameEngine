#pragma once
#include <cassert>
#include <string.h>
#include <stdlib.h>


namespace Engine
{
namespace JobSys
{

/**
 *	@brief "HashString" is a string that represented by a hash value. The string, in 
 *		   character format, is converted to the hash-value format using the FNV hash 
 *		   algorithm. 
 *		   For more detailed information about the FNV hash algorithm, please refer to 
 *		   the following link: http://isthe.com/chongo/tech/comp/fnv/
 */
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

	/* @brief Calculate the hash value of the given string object using FNV hash algorithm. */
	static inline unsigned int Hash(const char* str);

};


}//Namespace Engine
}//Namespace JobSystem

