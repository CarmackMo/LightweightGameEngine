/*
	This struct defines how results of function calls can be used
*/

#ifndef EAE6320_RESULTS_CRESULT_H
#define EAE6320_RESULTS_CRESULT_H

// Includes
//=========

#include <cstdint>

// Enumerations
//=============

namespace eae6320
{
	namespace Results
	{
		// Usually all that a caller cares about is whether a result is a success or a failure
		enum eResult : bool
		{
			IsSuccess = true,
			IsFailure = false
		};

		// The "severity" of a result is an unsigned 8-bit value where larger values correspond to bigger problems:
		namespace Severity
		{
			enum eSeverity : uint8_t
			{
				Success = 0,	// Nothing went wrong
				Warning = 1,	// This could be used with success or failure
				Default = 127,	// Something went wrong and the caller should take action accordingly
				Fatal = 255		// The program will exit or crash
			};
		}
		// Although the severities named above are explicitly provided
		// the actual severity in a result can be any unsigned 8-bit value
		// indicating how relatively severe it is compared to other results

		// A result can come from any "system" enumerated below:
		enum class eSystem : uint8_t
		{
			General,	// Generic results that any system can use

			Application,	// You can use this to create errors specific to your game
			Graphics,
			Logging,
			Platform,

			Count		// This must be last
		};
		static_assert( static_cast<uint8_t>( eSystem::Count ) <= 0x80, "A result's system ID must fit into 7 bits" );
	}
}

// Class Declaration
//==================

namespace eae6320
{
	class cResult
	{
		// Interface
		//==========

	public:

		// This class is just a wrapper for a 32 bit value,
		// where different bits have different meanings

		// Access
		//-------

		// The top bit indicates success if set (or failure if not set)
#define EAE6320_RESULTS_SUCCESS_MASK 0x80000000

		// A result will implicitly convert to true (success) or false (failure)
		constexpr operator bool() const { return IsSuccess(); }
		// An explicit function is also provided for cases where it makes code more readable
		constexpr bool IsSuccess() const { return ( value & EAE6320_RESULTS_SUCCESS_MASK ) != 0; }

		// The next 7 bits are the system that defined the result
#define EAE6320_RESULTS_SYSTEM_MASK 0x7f000000
#define EAE6320_RESULTS_SYSTEM_SHIFT 24

		constexpr Results::eSystem GetSystem() const
		{
			return static_cast<Results::eSystem>( ( value & EAE6320_RESULTS_SYSTEM_MASK ) >> EAE6320_RESULTS_SYSTEM_SHIFT );
		}

		// The next 8 bits are the relative severity of the result
#define EAE6320_RESULTS_SEVERITY_MASK 0x00ff0000
#define EAE6320_RESULTS_SEVERITY_SHIFT 16

		constexpr uint8_t GetSeverity() const
		{
			return static_cast<uint8_t>( ( value & EAE6320_RESULTS_SEVERITY_MASK ) >> EAE6320_RESULTS_SEVERITY_SHIFT );
		}

		// The final 16 bits are an arbitrary ID assigned by the system when defining the result.
		// No function is provided to get the ID;
		// instead, the caller is expected to compare the entire result against the named result, e.g.:
		//	if ( myResult == eae6320::Results::Success )

		constexpr bool operator ==( const cResult i_rhs ) const
		{
			return value == i_rhs.value;
		}

		constexpr bool operator !=( const cResult i_rhs ) const
		{
			return value != i_rhs.value;
		}

		// Initialization / Clean Up
		//--------------------------

		// Defines a result
		constexpr cResult( const Results::eResult i_isSuccess, const Results::eSystem i_system, const uint16_t i_id,
			const uint8_t i_severity = Results::Severity::Default )
			:
			value( ( static_cast<bool>( i_isSuccess ) ? EAE6320_RESULTS_SUCCESS_MASK : 0 )
				| ( static_cast<uint8_t>( i_system ) << EAE6320_RESULTS_SYSTEM_SHIFT )
				| ( i_severity << EAE6320_RESULTS_SEVERITY_SHIFT )
				| i_id )
		{

		}

		// An undefined result is treated as an error
		constexpr cResult()
			:
			cResult( Results::IsFailure, Results::eSystem::General, Results::Severity::Default, ~static_cast<uint16_t>( 0x0 ) )
		{

		}

#undef EAE6320_RESULTS_ID_UNDEFINED
#undef EAE6320_RESULTS_SUCCESS_MASK
#undef EAE6320_RESULTS_SYSTEM_MASK
#undef EAE6320_RESULTS_SYSTEM_SHIFT
#undef EAE6320_RESULTS_SEVERITY_MASK
#undef EAE6320_RESULTS_SEVERITY_SHIFT

		// Data
		//=====

	private:

		uint32_t value;
	};
}

#endif	// EAE6320_RESULTS_CRESULT_H
