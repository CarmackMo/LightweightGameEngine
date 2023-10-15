/*
	This is the base class for builders of specific asset types
	(meaning that MeshBuilder, ShaderBuilder, etc.,
	should all derive from this base class)
*/

#ifndef EAE6320_BUILDERHELPER_IBUILDER_H
#define EAE6320_BUILDERHELPER_IBUILDER_H

// Includes
//=========

#include <cstdlib>
#include <Engine/Results/Results.h>
#include <string>
#include <vector>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		// This only thing that a specific builder project's main() entry point should do
		// is to call the following function with the derived builder class
		// as the template argument:
			template<class tBuilder>
		int Build( char* const* i_arguments, const unsigned int i_argumentCount )
		{
			return tBuilder().ParseCommandArgumentsAndBuild( i_arguments, i_argumentCount ) ? EXIT_SUCCESS : EXIT_FAILURE;
		}

		class iBuilder
		{
			// Interface
			//==========

		public:

			// Build
			//------

			// The following function will be called from the templated Build<> function above
			// with the command line arguments directly from the main() entry point:
			cResult ParseCommandArgumentsAndBuild( char* const* i_arguments, const unsigned int i_argumentCount );

			// Initialization / Clean Up
			//--------------------------

			virtual ~iBuilder() = default;

			// Data
			//=====

		protected:

			const char* m_path_source = nullptr;
			const char* m_path_target = nullptr;

			// Inheritable Implementation
			//===========================

		private:

			// Build
			//------

			// ParseCommandArgumentsAndBuild() will extract the source and target paths
			// and then call this function in the derived class with any remaining (optional) arguments:
			virtual cResult Build( const std::vector<std::string>& i_optionalArguments ) = 0;
		};
	}
}

#endif	// EAE6320_BUILDERHELPER_IBUILDER_H
