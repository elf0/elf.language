#ifndef BINARY
#define BINARY

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Object.h"

namespace elf{namespace ast{

class Binary: public Object{
public:
//    enum class Operator{
//        opAddUnsigned, opAddInteger, opAddFloat,
//        opSubUnsigned, opSubInteger, opSubFloat,
//        opMulUnsigned, opMulInteger, opMulFloat,
//        opDivUnsigned, opDivInteger, opDivFloat,
//        opModUnsigned, opModInteger, opModFloat,
//        opShiftLeftUnsigned, opShiftLeftInteger,
//        opShiftRightUnsigned, opShiftRightInteger,
//        opAndUnsigned, opAndInteger,
//        opOrUnsigned, opOrInteger,
//        opXorUnsigned, opXorInteger,
//        opEqualUnsigned, opEqualInteger, opEqualFloat,
//        opNotEqualUnsigned, opNotEqualInteger, opNotEqualFloat,
//        opLessUnsigned, opLessInteger, opLessFloat,
//        opLessOrEqualUnsigned, opLessOrEqualInteger, opLessOrEqualFloat,
//        opGreaterUnsigned, opGreaterInteger, opGreaterFloat,
//        opGreaterOrEqualUnsigned, opGreaterOrEqualInteger, opGreaterOrEqualFloat
//    };

    Binary(Type op, Object *pLeft, Object *pRight)
        : Object(op)
//        , _op(op)
        , _pLeft(pLeft)
        , _pRight(pRight)
    {

    }

    ~Binary(){
        delete _pLeft;
        delete _pRight;
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Binary.new\n");
        s_osObjects.New("Binary");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Binary.delete\n");
        s_osObjects.Delete("Binary");
        ::operator delete(pObject);
    }
#endif

    //    Binary(Type type, Object *pLeft, Object *pRight)
    //        : Function(type)
    //        , _nInputs(2)
    //        , _pszInputs(new Arguement[2])
    //    {
    //        _pszInputs[0] = pLeft;
    //        _pszInputs[1] = pRight;
    //    }

//    Type getOperator()const{
//        return _op;
//    }

//    Type _op;
    Object *_pLeft;
    Object *_pRight;
};

}}//namespace elf.ast

#endif // BINARY

