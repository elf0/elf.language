#ifndef IF
#define IF


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Block.h"
//#include "Object.h"
//#include "Statement.h"

namespace elf{namespace ast{

//class Block;

class If: public Statement{
public:
    //    If(Object *pCondition, Statement *pTrue, Statement *pFalse)
    //        : Statement(Type::otIf)
    //        , _pCondition(pCondition)
    //        , _pTrue(pTrue)
    //        , _pFalse(pFalse)
    //    {}
    If(Object *pCondition)
        : Statement(Type::otIf)
        , _pCondition(pCondition)
        , _true("block.if.true")
        , _false("block.if.false")
    {
    }

    ~If(){
        delete _pCondition;
        //        delete _pTrue;
        //        delete _pFalse;
    }
#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "If.new\n");
        s_osObjects.New("If");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "If.delete\n");
        s_osObjects.Delete("If");
        ::operator delete(pObject);
    }
#endif
    Object *_pCondition = null;
    //    Statement *_pTrue = null;
    //    Statement *_pFalse = null;
    Block _true;
    Block _false;
};

}}//namespace elf.ast

#endif // IF

