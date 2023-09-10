#ifndef __LIB_STDINT_H
#define __LIB_STDINT_H
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

#define NULL ((void*)0)
#define DIV_ROUND_UP(X, STEP) ((X + STEP - 1) / (STEP))
#define bool int
#define true 1
#define false 0

#define PG_SIZE 4096

#define UNUSED __attribute__ ((unused))

#endif
