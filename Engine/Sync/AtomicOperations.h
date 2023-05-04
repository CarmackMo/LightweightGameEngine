#pragma once
#include "Dependency.h"

uint16_t AtomicIncrement(volatile uint16_t& i_Value);
uint16_t AtomicDecrement(volatile uint16_t& i_Value);

uint32_t AtomicIncrement(volatile uint32_t& i_Value);
uint32_t AtomicDecrement(volatile uint32_t& i_Value);

uint64_t AtomicIncrement(volatile uint64_t& i_Value);
uint64_t AtomicDecrement(volatile uint64_t& i_Value);