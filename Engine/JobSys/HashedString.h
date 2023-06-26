#pragma once

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
	HashedString();
	~HashedString();

	HashedString(const char* str);
	HashedString(const HashedString& other);
	HashedString& operator=(const HashedString& other);

	unsigned int Get() const;

	bool operator==(const HashedString& other) const;
	bool operator<(const HashedString& other) const;

	/* @brief Calculate the hash value of the given string object using FNV hash algorithm. */
	static unsigned int Hash(const char* str);

};


}//Namespace Engine
}//Namespace JobSystem

