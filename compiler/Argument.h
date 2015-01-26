#ifndef ARGUMENT
#define ARGUMENT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Variable.h"

namespace elf{namespace ast{

class Argument: public Variable{
public:
    Argument()
        : Variable(Type::otArgument)
    {}

    //    Argument(Type type)
    //        : Variable(type)
    //    {}

    Argument(const std::string &strName)
        : Variable(Type::otArgument, strName)
    {}

    //    Argument(Type type, const std::string &strName)
    //        : Variable(type, strName)
    //    {}
    ~Argument(){
    }
#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Argument.new\n");
        s_osObjects.New("Argument");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Argument.delete\n");
        s_osObjects.Delete("Argument");
        ::operator delete(pObject);
    }
#endif
    Type getArgumentType()const{
        return _atArgumentType;
    }

    void setArgumentType(Type type){
        _atArgumentType = type;
    }
private:
    Type _atArgumentType = Type::otUnknown;
};

class Arguments{
public:
    Arguments(U8 nCount)
        : _nCount(nCount)
        , _pszArguments(new Argument[nCount])
    {}

    Arguments(U8 nCount, Object::Type vtType)
        : _nCount(nCount)
        , _pszArguments(new Argument[nCount])
    {
        for(size_t i = 0; i < nCount; ++i)
            _pszArguments[i].setArgumentType(vtType);

    }

    ~Arguments(){
        delete []_pszArguments;
    }

    U8 getCount()const{
        return _nCount;
    }

    Argument &operator[](size_t nIndex)const{
        return _pszArguments[nIndex];
    }

private:
    U8 _nCount = 0;
    Argument* _pszArguments = null;
};

}}//namespace elf.ast

#endif // ARGUMENT

