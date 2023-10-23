/*
	This class can be used to automatically perform any task
	when a scope block ends.

	A typical usage with a lambda function would be:

		cScopeGuard scopeGuard( [&]()
			{
				// Clean up code here
			} );
*/

#ifndef EAE6320_CSCOPEGUARD_H
#define EAE6320_CSCOPEGUARD_H

namespace eae6320
{
	// Standard
	//=========

	// Once the scope guard is created
	// its assigned function will always be called
	// (i.e. it can't be disabled)

		template <typename tFunction>
	class cScopeGuard
	{
		// Interface
		//----------

	public:

		// Initialize / Clean Up
		cScopeGuard( tFunction&& i_function ) noexcept;
		~cScopeGuard();

		// Data
		//-----

	private:

		const tFunction m_function;

		// Implementation
		//---------------

	private:

		// Initialize / Clean Up
		cScopeGuard( const cScopeGuard& ) = delete;
		cScopeGuard( cScopeGuard&& ) = delete;
		cScopeGuard& operator =( const cScopeGuard& ) = delete;
		cScopeGuard& operator =( cScopeGuard&& ) = delete;
	};

	// Mutable
	//========

	// The scope guard can be disabled
	// (i.e. its assigned function will no longer be called)

		template <typename tFunction>
	class cScopeGuard_mutable
	{
		// Interface
		//----------

	public:

		void Disable() noexcept;

		// Initialize / Clean Up
		cScopeGuard_mutable( tFunction&& i_function ) noexcept;
		~cScopeGuard_mutable();

		// Data
		//-----

	private:

		const tFunction m_function;
		bool m_isEnabled = true;

		// Implementation
		//---------------

	private:

		// Initialize / Clean Up
		cScopeGuard_mutable( const cScopeGuard_mutable& ) = delete;
		cScopeGuard_mutable( cScopeGuard_mutable&& ) = delete;
		cScopeGuard_mutable& operator =( const cScopeGuard_mutable& ) = delete;
		cScopeGuard_mutable& operator =( cScopeGuard_mutable&& ) = delete;
	};
}

#include "cScopeGuard.inl"

#endif	// EAE6320_CSCOPEGUARD_H
