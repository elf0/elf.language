#ifndef RETURN
#define RETURN


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Block.h"

namespace elf{namespace ast{

class Return: public Statement{
public:
    Return()
        : Statement(Type::otReturn)
        //        , _block("block.return")
    {}

    Return(Object *pObject)
        : Statement(Type::otReturn)
        , _pObject(pObject)
        //        , _block("block.return")
    {
#ifdef DEBUG
        s_osObjects.New("Return");
#endif
    }

    ~Return(){
        delete _pObject;
#ifdef DEBUG
        s_osObjects.Delete("Return");
        fprintf(stderr, "~Return()\n");
#endif
    }

    Object *_pObject = null;
    //    Block _block;
};

}}//namespace elf.ast

#endif // RETURN

