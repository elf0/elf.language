#ifndef INTEGER
#define INTEGER

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Number.h"

namespace elf{namespace ast{

class Integer: public Number{
public:
    Integer(I8 nValue)
        : Number(Type::otInteger8)
        , _i8Value(nValue)
    {}

    Integer(I16 nValue)
        : Number(Type::otInteger16)
        , _i16Value(nValue)
    {}

    Integer(I32 nValue)
        : Number(Type::otInteger32)
        , _i32Value(nValue)
    {}

    Integer(I64 nValue)
        : Number(Type::otInteger64)
        , _i64Value(nValue)
    {}
    //    enum class Type{
    //        Integer8, Integer16, Integer32, Integer64
    //    };

    ~Integer(){
#ifdef DEBUG
        fprintf(stderr, "~Integer(%lld)\n", _i64Value);
#endif
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Integer.new\n");
        s_osObjects.New("Integer");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Integer.delete\n");
        s_osObjects.Delete("Integer");
        ::operator delete(pObject);
    }
#endif
//    enum class Binary{
//        Extend, Truncate,
//        Assign, Add, Sub, Mul, Div, Mod,
//        ShiftLeft, ShiftRight,
//        And, Or, Not, Xor,
//        Equal, NotEqaul, Less, LessOrEqaul, Greater, GreaterOrEqaul,
//    };


    I8 getI8(){
        return _i8Value;
    }

    I16 getI16(){
        return _i16Value;
    }

    I32 getI32(){
        return _i32Value;
    }

    I64 getI64(){
        return _i64Value;
    }

    void setValue(I8 value){
        _i8Value = value;
    }

    void setValue(I16 value){
        _i16Value = value;
    }

    void setValue(I32 value){
        _i32Value = value;
    }

    void setValue(I64 value){
        _i64Value = value;
    }
    //    Type _type;
    //    Binary _bin;
    union{
        I8 _i8Value;
        I16 _i16Value;
        I32 _i32Value;
        I64 _i64Value;
    };
    //    Bool _bKnown = false;
    //    Bool _bConstant;
};

}}//namespace elf.ast

#endif // INTEGER

