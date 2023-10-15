#ifndef EAE6320_CSCOPEGUARD_INL
#define EAE6320_CSCOPEGUARD_INL

// Includes
//=========

#include "cScopeGuard.h"

#include <utility>

// Standard
//=========

// Interface
//----------

// Initialize / Clean Up

	template <typename tFunction>
eae6320::cScopeGuard<tFunction>::cScopeGuard( tFunction&& i_function ) noexcept
	:
	m_function( std::forward<tFunction>( i_function ) )
{

}

	template <typename tFunction>
eae6320::cScopeGuard<tFunction>::~cScopeGuard() noexcept
{
	m_function();
}

// Mutable
//========

// Interface
//----------

	template <typename tFunction>
void eae6320::cScopeGuard_mutable<tFunction>::Disable() noexcept
{
	m_isEnabled = false;
}

// Initialize / Clean Up

	template <typename tFunction>
eae6320::cScopeGuard_mutable<tFunction>::cScopeGuard_mutable( tFunction&& i_function ) noexcept
	:
	m_function( std::forward<tFunction>( i_function ) )
{

}

	template <typename tFunction>
eae6320::cScopeGuard_mutable<tFunction>::~cScopeGuard_mutable() noexcept
{
	if ( m_isEnabled )
	{
		m_function();
	}
}

#endif	// EAE6320_CSCOPEGUARD_INL
