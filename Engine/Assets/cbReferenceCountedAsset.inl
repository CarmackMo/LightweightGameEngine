#ifndef EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_INL
#define EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_INL

// Include Files
//==============

#include "cbReferenceCountedAsset.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

#ifdef EAE6320_ASSETS_SHOULDREFERENCECOUNTBECHECKEDINDESTRUCTOR

	// Implementation
	//===============

	// Initialization / Clean Up
	//--------------------------

	template <class tAsset>
		inline eae6320::Assets::cbReferenceCountedAsset<tAsset>::~cbReferenceCountedAsset()
	{
		EAE6320_ASSERTF( m_referenceCount == 0, "A reference counted asset is being deleted with a non-zero reference count (%u)", m_referenceCount );
		eae6320::Logging::OutputError( "A reference counted asset was destructed with a non-zero reference count (%u)", m_referenceCount );
	}

#endif	// EAE6320_ASSETS_SHOULDREFERENCECOUNTBECHECKEDINDESTRUCTOR

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Windows/cbReferenceCountedAsset.win.inl"
#endif

#endif	// EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_INL
