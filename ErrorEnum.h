#ifndef ERROR_ENUM_H
#define ERROR_ENUM_H

#include <iostream>

enum class RESULT_VALUE : uint8_t
{
	OK = 0,
	GENERIC_ERROR,
	ALLOCATOR_NOT_INITIALIZED,
	ERROR_DOUBLE_FREE,
};

#endif