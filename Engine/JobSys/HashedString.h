#pragma once
#include "Dependency.h"

class HashedString
{
private:
	unsigned int 	m_Hash;

#ifdef DEBUG_KEEP_STRING
	const char* m_pString;
#endif


public:
	inline HashedString();
	inline ~HashedString();

	inline HashedString(const char* i_string);
	inline HashedString(const HashedString& i_other);
	inline HashedString& operator=(const HashedString& i_other);

	inline unsigned int Get() const;

	inline bool operator==(const HashedString& i_other) const;
	inline bool operator<(const HashedString& i_other) const;

	static unsigned int Hash(const char* i_string);
	static unsigned int Hash(const void* i_bytes, size_t i_count);
};

#include "HashedString.inl"