/*****************************************************************
 * This file is part of JustOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the terms of the Social Justice License        *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _TYPES_H
#define _TYPES_H

// simple typedefs for generic types that are needed throughout kernel
// definitions for more complex or subsystem-specific types should go in the
// header for the appropriate subsystem

typedef unsigned char uint8_t;
typedef char int8_t;

typedef unsigned short int uint16_t;
typedef short int int16_t;

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned long int uint64_t;
#define UINT64_T_MAX 0xFFFFFFFFFFFFFFFF
typedef long int int64_t;

typedef enum bool{
	false = 0,
	true = 1
} bool;
#endif