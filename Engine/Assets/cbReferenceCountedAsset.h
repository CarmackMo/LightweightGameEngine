/*
	This is a base class for assets that provides reference counting functionality
*/

#ifndef EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_H
#define EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_H

// Include Files
//==============

#include <cstdint>

// Configuration
//==============

// A sanity check can be made to ensure that the destructor isn't called before it should be
#ifdef _DEBUG
	#define EAE6320_ASSETS_SHOULDREFERENCECOUNTBECHECKEDINDESTRUCTOR
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		// tAsset must be the class that inherits from this base class
		// (using templates allows this base class to know how to delete itself properly
		// without requiring virtual inheritance)
		template <class tAsset>
			class cbReferenceCountedAsset
		{
			// Interface
			//==========

		public:

			// Reference Counting
			//-------------------

			void IncrementReferenceCount();
			void DecrementReferenceCount();

			// Initialization / Clean Up
			//--------------------------

			// Reference-counted assets can't be copied or assigned to
			cbReferenceCountedAsset( const cbReferenceCountedAsset& i_instanceToBeCopied ) = delete;
			cbReferenceCountedAsset& operator =( const cbReferenceCountedAsset& i_instanceToBeCopied ) = delete;
			cbReferenceCountedAsset( cbReferenceCountedAsset&& i_instanceToBeMoved ) = delete;
			cbReferenceCountedAsset& operator =( cbReferenceCountedAsset&& i_instanceToBeMoved ) = delete;

			// Data
			//=====

		private:

			uint32_t m_referenceCount = 1;

			// Inherited Implementation
			//=========================

		protected:

			// Initialization / Clean Up
			//--------------------------

			// In a derived class:
			//	* The constructor should not be public unless initialization is trivial (and it won't be for our class)
			//		and a factory function should be used to construct/initialize a new instance
			//	* The destructor should not be public
			//		(a class instance should only be destructed automatically when its reference count goes to zero)
			//		* This means that this base class must be made a friend of the derived class
			cbReferenceCountedAsset() = default;
			~cbReferenceCountedAsset()
#ifndef EAE6320_ASSETS_SHOULDREFERENCECOUNTBECHECKEDINDESTRUCTOR
				= default
#endif
				;
		};
	}
}

#include "cbReferenceCountedAsset.inl"

#endif	// EAE6320_ASSETS_CBREFERENCECOUNTEDASSET_H
