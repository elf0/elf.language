#ifndef NUMBER
#define NUMBER


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Object.h"

namespace elf{namespace ast{

class Number: public Object{
public:
    Number(Type type)
        : Object(type)
    {}

//    enum class Type{
//        Unsigned8, Unsigned16, Unsigned32, Unsigned64,
//        Integer8, Integer16, Integer32, Integer64,
//        Float32, Float64,
//    };

//    enum class Operator{
//        Assign, Add, Sub, Mul, Div, Mod,
//        And, Or, Not, Xor,
//        Equal, NotEqaul, Less, LessOrEqaul, Greater, GreaterOrEqaul,
//        ShiftLeft, ShiftRight,
//    };
//    Type _type;
//    Operator _op;
//    union{
//        Bool bValue;
//        U8 u8Value;
//        U16 u16Value;
//        U32 u32Value;
//        U64 u64Value;
//        I8 i8Value;
//        I16 i16Value;
//        I32 i32Value;
//        I64 iValue;
//        F32 f32Value;
//        F64 f64Value;
//    };
    Bool bKnown;
    Bool bConstant;
};

}}//namespace elf.ast

#endif // NUMBER

