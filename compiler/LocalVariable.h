#ifndef LOCALVARIABLE
#define LOCALVARIABLE


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Statement.h"

namespace elf{namespace ast{

class LocalVariable: public Object{
public:
    LocalVariable(Type type, const std::string &strName)
        : Object(Type::otLocalVariable)
        , _otVariableType(type)
        , _strName(strName)
    {}

    ~LocalVariable(){
#ifdef DEBUG
        fprintf(stderr, "~LocalVariable.\"%s\"()\n", _strName.c_str());
#endif
    }
#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "LocalVariable.new\n");
        s_osObjects.New("LocalVariable");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "LocalVariable.delete\n");
        s_osObjects.Delete("LocalVariable");
        ::operator delete(pObject);
    }
#endif
    Type getVariableType(){
        return _otVariableType;
    }

    const std::string &getName()const{
        return _strName;
    }

private:
    Type _otVariableType;
    std::string _strName;
};

class AllocateLocalVariable: public Statement{
public:
    AllocateLocalVariable(LocalVariable *plvVariable)
        : Statement(Type::otAllocateLocalVariable)
        , _plvVariable(plvVariable)
    {}

    ~AllocateLocalVariable(){
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "AllocateLocalVariable.new\n");
        s_osObjects.New("AllocateLocalVariable");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "AllocateLocalVariable.delete\n");
        s_osObjects.Delete("AllocateLocalVariable");
        ::operator delete(pObject);
    }
#endif
    LocalVariable *_plvVariable;
//    Type _otVariableType;
};

class ReadLocalVariable: public Statement{
public:
    ReadLocalVariable(LocalVariable *plvVariable)
        : Statement(Type::otReadLocalVariable)
        , _plvVariable(plvVariable)
    {}

    ~ReadLocalVariable(){
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "ReadLocalVariable.new\n");
        s_osObjects.New("ReadLocalVariable");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "ReadLocalVariable.delete\n");
        s_osObjects.Delete("ReadLocalVariable");
        ::operator delete(pObject);
    }
#endif
    LocalVariable *_plvVariable;
};

class WriteLocalVariable: public Statement{
public:
    WriteLocalVariable(LocalVariable *plvVariable, Object *pValue)
        : Statement(Type::otWriteLocalVariable)
        , _plvVariable(plvVariable)
        , _pValue(pValue)
    {}

    ~WriteLocalVariable(){
        delete _pValue;
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "WriteLocalVariable.new\n");
        s_osObjects.New("WriteLocalVariable");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "WriteLocalVariable.delete\n");
        s_osObjects.Delete("WriteLocalVariable");
        ::operator delete(pObject);
    }
#endif
    LocalVariable *_plvVariable;
    Object *_pValue;
};

}}//namespace elf.ast

#endif // LOCALVARIABLE

