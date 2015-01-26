#ifndef VARIABLE
#define VARIABLE


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Object.h"

namespace elf{namespace ast{

class Variable: public Object{
public:
    Variable()
        : Object(Type::otVariable)
    {}

    Variable(Type type)
        : Object(type)
    {}

    Variable(const std::string &strName)
        : Variable(Type::otVariable, strName)
    {}

    Variable(Type type, const std::string &strName)
        : Object(type)
        , _strName(strName)
    {}

    const std::string &getName()const{
        return _strName;
    }

    void setName(const std::string &strName){
        _strName = strName;
    }
private:
    std::string _strName;
};

}}//namespace elf.ast

#endif // VARIABLE

