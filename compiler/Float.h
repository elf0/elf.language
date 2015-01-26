#ifndef FLOAT
#define FLOAT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Number.h"

namespace elf{namespace ast{

class Float: public Number{
public:
    Float(F32 fValue)
        : Number(Type::otFloat32)
        , _f32Value(fValue)
    {}

    Float(F64 fValue)
        : Number(Type::otFloat64)
        , _f64Value(fValue)
    {}

    ~Float(){
#ifdef DEBUG
        fprintf(stderr, "~Float(%f)\n", getType() == Type::otFloat32? _f32Value : _f64Value);
#endif
    }
#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Float.new\n");
        s_osObjects.New("Float");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Float.delete\n");
        s_osObjects.Delete("Float");
        ::operator delete(pObject);
    }
#endif

    F32 getValue32(){
        return _f32Value;
    }

    F64 getValue64(){
        return _f64Value;
    }
//    enum class Type{
//        Float32, Float64
//    };

//    enum class Operator{
//        Assign, Add, Sub, Mul, Div, Mod,
//        Equal, NotEqaul, Less, LessOrEqaul, Greater, GreaterOrEqaul,
//    };
//    Type _type;
//    Operator _op;
    union{
        F32 _f32Value;
        F64 _f64Value;
    };
//    Bool _bKnown;
//    Bool _bConstant;
};

}}//namespace elf.ast

#endif // FLOAT

