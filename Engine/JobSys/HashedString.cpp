#include "HashedString.h"




inline HashedString::HashedString() :
	m_Hash(Hash(""))
{ }

inline HashedString::HashedString(const char* i_string) :
	m_Hash(Hash(i_string))
#ifdef DEBUG_KEEP_STRING
	, m_pString(strdup(i_string))
#endif
{ }

inline HashedString::HashedString(const HashedString& i_other) :
	m_Hash(i_other.m_Hash)
#ifdef DEBUG_KEEP_STRING
	, m_pString(strdup(i_other.m_pString))
#endif
{ }

inline HashedString::~HashedString()
{
#ifdef DEBUG_KEEP_STRING
	if (m_pString)
		free(m_pString)
#endif

}

inline HashedString& HashedString::operator=(const HashedString& i_other)
{
	m_Hash = i_other.m_Hash;

#ifdef DEBUG_KEEP_STRING
	if (m_pString)
		free(m_pString)

		m_pString = i_other.m_pString;
#endif

	return *this;
}

inline unsigned int HashedString::Get(void) const
{
	return m_Hash;
}

inline bool HashedString::operator==(const HashedString& i_other) const
{
	return m_Hash == i_other.m_Hash;
}

inline bool HashedString::operator<(const HashedString& i_other) const
{
	return m_Hash < i_other.m_Hash;
}


unsigned int HashedString::Hash(const char* i_string)
{
	assert(i_string);

	return Hash(reinterpret_cast<void*>(const_cast<char*>(i_string)), strlen(i_string));
}

unsigned int HashedString::Hash(const void* i_bytes, size_t i_bytecount)
{
	// FNV hash, http://isthe.com/chongo/tech/comp/fnv/

	register const unsigned char* p = static_cast<const unsigned char*>(i_bytes);
	unsigned int hash = 2166136261;

	for (size_t i = 0; i < i_bytecount; ++i)
		hash = 16777619 * (hash ^ p[i]);

	return hash ^ (hash >> 16);
}
