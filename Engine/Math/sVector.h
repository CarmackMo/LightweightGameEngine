/*
	This struct represents a three-dimensional position or direction
*/

#ifndef EAE6320_MATH_SVECTOR_H
#define EAE6320_MATH_SVECTOR_H

// Struct Declaration
//===================

namespace eae6320
{
	namespace Math
	{
		struct sVector
		{
			// Data
			//=====

			float x = 0.0f, y = 0.0f, z = 0.0f;

			// Interface
			//==========

			// Addition
			//---------

			constexpr sVector operator +( const sVector& i_rhs ) const;
			constexpr sVector& operator +=( const sVector& i_rhs );

			constexpr sVector operator +( const float i_rhs ) const;
			constexpr sVector& operator +=( const float i_rhs );
			friend constexpr sVector operator +( const float i_lhs, const sVector& i_rhs );


			// Subtraction / Negation
			//-----------------------

			constexpr sVector operator -( const sVector& i_rhs ) const;
			constexpr sVector& operator -=( const sVector& i_rhs );
			constexpr sVector operator -() const;

			constexpr sVector operator -( const float i_rhs ) const;
			constexpr sVector& operator -=( const float i_rhs );
			friend constexpr sVector operator -( const float i_lhs, const sVector& i_rhs );

			// Products
			//---------

			constexpr sVector operator *( const float i_rhs ) const;
			constexpr sVector& operator *=( const float i_rhs );
			friend constexpr sVector operator *( const float i_lhs, const sVector& i_rhs );

			friend constexpr float Dot( const sVector& i_lhs, const sVector& i_rhs );
			friend constexpr sVector Cross( const sVector& i_lhs, const sVector& i_rhs );

			// Division
			//---------

			sVector operator /( const float i_rhs ) const;
			sVector& operator /=( const float i_rhs );

			// Length / Normalization
			//-----------------------

			float GetLength() const;
			float Normalize();
			sVector GetNormalized() const;

			// Comparison
			//-----------

			constexpr bool operator ==( const sVector& i_rhs ) const;
			constexpr bool operator !=( const sVector& i_rhs ) const;

			// Initialization / Clean Up
			//--------------------------

			constexpr sVector() = default;
			constexpr sVector( const float i_x, const float i_y, const float i_z );
		};

		// Friends
		//========

		constexpr sVector operator +( const float i_lhs, const sVector& i_rhs );
		constexpr sVector operator -( const float i_lhs, const sVector& i_rhs );
		constexpr sVector operator *( const float i_lhs, const sVector& i_rhs );
		constexpr float Dot( const sVector& i_lhs, const sVector& i_rhs );
		constexpr sVector Cross( const sVector& i_lhs, const sVector& i_rhs );
	}
}

#include "sVector.inl"

#endif	// EAE6320_MATH_SVECTOR_H
