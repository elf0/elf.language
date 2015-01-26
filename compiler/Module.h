#ifndef MODULE
#define MODULE

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <string>
#include "Object.h"

namespace elf{namespace ast{

class Module: public Object{
public:
    Module(const std::string &strName)
        : Object(Type::otModule)
        , _strName(strName)
    {
    }

    ~Module(){
        for(auto pr: _mapObjects)
            delete pr.second;
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Module.new\n");
        s_osObjects.New("Module");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Module.delete\n");
        s_osObjects.Delete("Module");
        ::operator delete(pObject);
    }
#endif
    //Caller must release pNewObject, if failed.
    Bool Insert(const std::string &strName, Object *pNewObject){
        Object *&pObject = _mapObjects[strName];
        if(pObject != null)
            return false;
        pObject = pNewObject;
        return true;
    }

    const std::string &getName()const{
        return _strName;
    }

    void setName(const std::string &strName){
        _strName = strName;
    }

    //    const Module *FindModule(const std::string &strName)const{
    //        auto iter = _mapModules.find(strName);
    //        if(iter == _mapModules.end())
    //            return null;
    //        return iter->second;
    //    }

    //    std::map<std::string, Module*> _mapModules;
    //    std::map<std::string, Unsigned*> _mapConstantUnsigneds;
    //Constant
    //v: 1
    //v: 1.0
    std::map<std::string, Object*> _mapObjects;
    //    std::map<std::string, I64> _mapIntegers;
    //    std::map<std::string, F64> _mapFloats;
    //    std::map<std::string, std::string> _mapStrings;
    //    std::map<std::string, Function*> _mapFunctions;
    //    std::map<std::string, Class*> _mapClasses;
    //    std::set<std::string> _setSymbols;
private:
    std::string _strName;
};

}}//namespace elf.ast

#endif // MODULE

