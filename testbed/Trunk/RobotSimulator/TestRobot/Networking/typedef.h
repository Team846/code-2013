#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#ifdef __VXWORKS__
#else
#include <stdint.h>

#define NULL		0

typedef int	(*FUNCPTR) (...);

#ifndef UINT8_DEF_
#define UINT8_DEF_
typedef	uint8_t		UINT8;
#endif

#ifndef UINT16_DEF_
#define UINT16_DEF_
typedef uint16_t	UINT16;
#endif

#ifndef UINT32_DEF_
#define UINT32_DEF_
typedef uint32_t	UINT32;
#endif

#ifndef UINT64_DEF_
#define UINT64_DEF_
typedef uint64_t	UINT64;
#endif

#ifndef INT8_DEF_
#define INT8_DEF_
typedef int8_t		INT8;
#endif

#ifndef INT16_DEF_
#define INT16_DEF_
typedef int16_t		INT16;
#endif

// sketchy workaround for libjpeg
#define XMD_H
#ifndef INT32_DEF_
#define INT32_DEF_
typedef int32_t		INT32;
#endif
// Fix issues with libjpeg
#ifndef INT64_DEF_
#define INT64_DEF_
typedef int64_t		INT64;
#endif

#ifndef ULONG_DEF_
#define ULONG_DEF_
typedef uint64_t	ULONG;
#endif

typedef int			STATUS;
#endif

#endif
