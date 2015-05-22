#ifndef _PBMS_COMMONTYPES_H_
#define _PBMS_COMMONTYPES_H_


/**
 * Cross platform shorthand data types, primarily used in Torque.
 * NOTE: These type defines are 32bit/64bit x86 arch biased.
 */

/*
typedef signed char      S8;
typedef signed short     S16;
typedef signed int       S32;
typedef signed long long S64;

typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;
*/

#include <stdint.h>

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float  F32;
typedef double F64;


#endif // _PBMS_COMMONTYPES_H_


