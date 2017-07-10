/*
 * MHJDefine.h
 *
 *  Created on: 2015骞�7鏈�4鏃�
 *      Author: root
 */

#ifndef HEADER_MHJDEFINE_H_
#define HEADER_MHJDEFINE_H_

//android瀹氫箟
#ifdef ANDROID
typedef unsigned int uint;
 typedef uint in_addr_t;

#endif
//android瀹氫箟

#ifdef WIN32
#include <stdint.h>
#include<windows.h>
typedef uint32_t in_addr_t;
#define  uint unsigned short
#endif

#ifdef LINUX
#include <stdint.h>
#include <sys/types.h>

#else
#include <c_types.h>
typedef unsigned int uint;

//#ifdef ESP6288
//typedef unsigned int uint32_t;
//typedef int int32_t;
//#else
////#include "c_types.h"
//#endif

typedef uint in_addr_t;
typedef short int16_t;
typedef unsigned short uint16_t;

#define os_malloc   pvPortMalloc
#define os_free     vPortFree
#define os_zalloc   pvPortZalloc

#define malloc   pvPortMalloc
#define free     vPortFree
#define sprintf	ets_sprintf
//#define zalloc   pvPortZalloc

#define os_memcpy ets_memcpy
#define memcpy ets_memcpy
#define memset ets_memset
#endif


#ifndef WIN32

typedef int16_t INT16;
typedef uint16_t UINT16;
typedef uint INT32;
typedef unsigned char BYTE;
typedef UINT16 WORD;
typedef uint DWORD;
#endif

#endif /* HEADER_MHJDEFINE_H_ */
