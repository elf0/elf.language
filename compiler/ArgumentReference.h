#ifndef ARGUMENTREFERENCE
#define ARGUMENTREFERENCE

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Argument.h"

namespace elf{namespace ast{

class ArgumentReference: public Object{
public:

    //    ArgumentReference(U8 nIndex)
    //        : Object(Type::otArgumentReference)
    //        , _nIndex(nIndex)
    //    {}

    ~ArgumentReference(){
        //        fprintf(stderr, "~ArgumentReference(%u)\n", _nIndex);
#ifdef DEBUG
        fprintf(stderr, "~ArgumentReference(%p)\n", _pArgument);
#endif

    }

    Argument *getArgument()const{
        return _pArgument;
    }

    //    U8 getIndex()const{
    //        return _nIndex;
    //    }
protected:
    //    ArgumentReference(Type type, U8 nIndex)
    //        : Object(type)
    //        , _nIndex(nIndex)
    //    {}
    ArgumentReference(Type type, Argument *pArgument)
        : Object(type)
        , _pArgument(pArgument)
    {}
    //    U8 _nIndex = 0xFF;
    Argument *_pArgument;
};

class InputReference: public ArgumentReference{
public:
//    InputReference(U8 nIndex)
//        : ArgumentReference(Type::otInputReference, nIndex)
//    {}
    InputReference(Argument *pArgument)
        : ArgumentReference(Type::otInputReference, pArgument)
    {}

    ~InputReference(){
#ifdef DEBUG
        fprintf(stderr, "~InputReference(%p)\n", _pArgument);
#endif
    }
};

class OutputReference: public ArgumentReference{
public:
//    OutputReference(U8 nIndex)
//        : ArgumentReference(Type::otOutputReference, nIndex)
//    {}
    OutputReference(Argument *pArgument)
        : ArgumentReference(Type::otOutputReference, pArgument)
    {}

    ~OutputReference(){
#ifdef DEBUG
        fprintf(stderr, "~OutputReference(%p)\n", _pArgument);
#endif
    }
};

}}//elf.ast

#endif // ARGUMENTREFERENCE

