/*
	A handle is an opaque identifier associated with an asset loaded from a file

	It is used similarly to a pointer
	in that it is a small value that can be used indirectly to get to a bigger value.
	An asset manager will return a handle when it is asked to load an asset,
	and an asset manager can be given a handle to access the loaded asset's actual pointer.
*/

#ifndef EAE6320_ASSETS_CHANDLE_H
#define EAE6320_ASSETS_CHANDLE_H

// Includes
//=========

#include <cstdint>
#include <Engine/Results/Results.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Assets
	{
		template <class tAsset, class tKey> class cManager;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		// Handles are templated for type safety:
		// A handle for one kind of asset can't be used for a different kind of asset
			template <class tAsset>
		class cHandle
		{
			// Interface
			//==========

		public:

			// Access
			//-------

			// The handle class doesn't provide access to its associated asset;
			// instead, you must pass the handle to the appropriate asset manager

			bool IsValid() const { return GetIndex() != InvalidIndex; }
			operator bool() const { return IsValid(); }

			// The index can be retrieved for unsafe operations cManager<tAsset> operations
#define EAE6320_ASSETS_INDEX_MASK 0Xfffff	// The lowest 20 bits
			uint_fast32_t GetIndex() const { return static_cast<uint_fast32_t>( m_value & EAE6320_ASSETS_INDEX_MASK ); }

			// Initialize / Clean Up
			//----------------------

			cHandle() = default;

			// Comparison
			//-----------

			bool operator ==( const cHandle i_lhs ) const { return m_value == i_lhs.m_value; }
			bool operator !=( const cHandle i_lhs ) const { return m_value != i_lhs.m_value; }

			// Data
			//=====

		private:

			// This class is just a wrapper for a 32 bit value,
			// where different bits have different meanings

			// The handle's index and error checking ID together use 32 bits,
			// but there is a tradeoff in how those bits are divided between the two:
			// The more bits that the index has increases the total number of assets that can exist at any one time,
			// but the fewer bits that the error checking ID has increases the potential of collisions due to overflow.

			// The largest possible bit value is used as an invalid index

			static constexpr uint_fast32_t InvalidIndex = EAE6320_ASSETS_INDEX_MASK;
			uint32_t m_value = InvalidIndex;

			// Implementation
			//===============

		private:

#define EAE6320_ASSETS_ID_SHIFT 20	// The remaining high bits
#define EAE6320_ASSETS_ID_MAX ( 1 << ( ( sizeof( cHandle<void*> ) * 8 ) - EAE6320_ASSETS_ID_SHIFT ) ) - 1

			uint_fast16_t GetId() const { return static_cast<uint_fast16_t>( m_value >> EAE6320_ASSETS_ID_SHIFT ); }

			void MakeInvalid() { m_value = InvalidIndex; }
			static constexpr uint_fast16_t IncrementId( const uint_fast16_t i_id )
			{
				// The ID wraps around after it runs out of bits
				return ( i_id + 1 ) & EAE6320_ASSETS_ID_MAX;
			}

			// Initialize / Clean Up
			//----------------------

			cHandle( const uint_fast32_t i_index, const uint_fast16_t i_id )
				:
				m_value( i_index | ( i_id << EAE6320_ASSETS_ID_SHIFT ) )
			{
				static_assert( ( ( sizeof( cHandle<void*> ) * 8 ) - EAE6320_ASSETS_ID_SHIFT ) <= 16,
					"A handle's error checking ID must fit into 16 bits" );
			}

#undef EAE6320_ASSETS_INDEX_MASK
#undef EAE6320_ASSETS_ID_SHIFT
#undef EAE6320_ASSETS_ID_MAX

			// Friends
			//========

			// Nothing should ever worry about the IDs except asset managers
			template <class tAsset, class tKey> friend class cManager;
		};
	}
};

#endif	// EAE6320_ASSETS_CHANDLE_H
