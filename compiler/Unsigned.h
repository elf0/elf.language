#ifndef UNSIGNED
#define UNSIGNED


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Number.h"

namespace elf{namespace ast{

class Unsigned: public Number{
public:
        Unsigned(U8 nValue)
        : Number(Type::otUnsigned8)
        , _u8Value(nValue)
    {}

    Unsigned(U16 nValue)
        : Number(Type::otUnsigned16)
        , _u16Value(nValue)
    {}

    Unsigned(U32 nValue)
        : Number(Type::otUnsigned32)
        , _u32Value(nValue)
    {}

    Unsigned(U64 nValue)
        : Number(Type::otUnsigned64)
        , _u64Value(nValue)
    {}


#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Unsigned.new\n");
        s_osObjects.New("Unsigned");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Unsigned.delete\n");
        s_osObjects.Delete("Unsigned");
        ::operator delete(pObject);
    }
#endif
//    enum class Type{
//        Unsigned8, Unsigned16, Unsigned32, Unsigned64
//    };

//    enum class Binary{
//        Extend, Truncate,
//        Assign, Add, Sub, Mul, Div, Mod,
//        ShiftLeft, ShiftRight,
//        And, Or, Not, Xor,
//        Equal, NotEqaul, Less, LessOrEqaul, Greater, GreaterOrEqaul,
//    };

//    Type _type;
//    Binary _bin;
//    Bool getBool(){
//        return _u8Value;
//    }

    U8 getU8(){
        return _u8Value;
    }

    U16 getU16(){
        return _u16Value;
    }

    U32 getU32(){
        return _u32Value;
    }

    U64 getU64(){
        return _u64Value;
    }

    void setValue(U8 value){
        _u8Value = value;
    }

    void setValue(U16 value){
        _u16Value = value;
    }

    void setValue(U32 value){
        _u32Value = value;
    }

    void setValue(U64 value){
        _u64Value = value;
    }

    union{
        U8 _u8Value;
        U16 _u16Value;
        U32 _u32Value;
        U64 _u64Value;
    };
//    Bool _bKnown;
//    Bool _bConstant;
};

}}//namespace elf.ast

#endif // UNSIGNED

