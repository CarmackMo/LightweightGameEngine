#ifndef EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_WIN_INL
#define EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_WIN_INL

// Include Files
//==============

#include "../cbReferenceCountedAsset.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Windows/Includes.h>

// Interface
//==========

// Reference Counting
//-------------------

template <class tAsset>
	inline void eae6320::Assets::cbReferenceCountedAsset<tAsset>::IncrementReferenceCount()
{
	EAE6320_ASSERT( m_referenceCount > 0 );
	EAE6320_ASSERT( ( reinterpret_cast<uintptr_t>( &m_referenceCount ) % 4 ) == 0 );	// The reference count must be 32-bit aligned
	// InterlockedIncrementNoFence() is atomic:
	// If two threads call it at the same time there is no guarantee which thread will increment it first,
	// but it is guaranteed that it will be incremented exactly two times
	InterlockedIncrementNoFence( &m_referenceCount );
}

template <class tAsset>
	inline void eae6320::Assets::cbReferenceCountedAsset<tAsset>::DecrementReferenceCount()
{
	EAE6320_ASSERT( m_referenceCount > 0 );
	EAE6320_ASSERT( ( reinterpret_cast<uintptr_t>( &m_referenceCount ) % 4 ) == 0 );	// The reference count must be 32-bit aligned
	// InterlockedDecrementNoFence() is atomic:
	// If two threads call it at the same time there is no guarantee which thread will decrement it first,
	// but it is guaranteed that it will be decremented exactly two times
	// and that the returned decremented reference count will be unique in each of those threads
	// (and, more importantly, will only be zero in exactly one of those threads
	// which means that the reference-counted asset will only be deleted once)
	const auto newReferenceCount = InterlockedDecrementNoFence( &m_referenceCount );
	// If no more references are being held to this asset it is deleted
	if ( newReferenceCount == 0 )
	{
		delete static_cast<tAsset*>( this );
	}
}

#endif	// EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_WIN_INL
