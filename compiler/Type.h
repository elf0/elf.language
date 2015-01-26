#ifndef TYPE_H
#define TYPE_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

typedef unsigned char      Byte;
typedef unsigned char      Char;

typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;
typedef signed long long  I64;

typedef float              F32;
typedef double             F64;

typedef U8 Bool;
#define false 0
#define true  1

typedef Byte* Pointer;
#define null  0

typedef signed long Result;

//namespace elf{
//class Type{
//public:
//    enum class Type{
//        ttBoolean,
//        ttUnsigned8, ttUnsigned16, ttUnsigned32, ttUnsigned64,
//        ttInteger8, ttInteger16, ttInteger32, ttInteger64,
//        ttFloat32, ttFloat64
//    };
//};
//}//namespace elf

#endif //TYPE_H
