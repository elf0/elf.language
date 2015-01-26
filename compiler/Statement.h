#ifndef STATEMENT
#define STATEMENT


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Object.h"

namespace elf{namespace ast{

class Statement: public Object{
public:
    Statement(Type type)
        : Object(type)
    {}
};

}}//namespace elf.ast

#endif // STATEMENT

