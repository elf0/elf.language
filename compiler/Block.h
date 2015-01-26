#ifndef BLOCK
#define BLOCK

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <string>
#include <list>
#include <map>



#include "Statement.h"
#include "LocalVariable.h"

#include "Binary.h"
#include "Return.h"
//#include "If.h"
#include "Call.h"

namespace elf{namespace ast{

//class Call;

class Block: public Statement{
public:
    Block(const std::string &strName, Block *pParent)
        : Statement(Type::otBlock)
        , _strName(strName)
        , _pParent(pParent)
    {}

    Block(const std::string &strName)
        : Statement(Type::otBlock)
        , _strName(strName)
    {}

    ~Block(){
        for(auto p: _lstStatements)
            delete p;

        for(auto pr: _mapLocalVariables)
            delete pr.second;

#ifdef DEBUG
        fprintf(stderr, "~Block.\"%s\"()\n", _strName.c_str());
#endif
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Block.new\n");
        s_osObjects.New("Block");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Block.delete\n");
        s_osObjects.Delete("Block");
        ::operator delete(pObject);
    }
#endif
    LocalVariable *PushLocalVariable(const std::string &strName, Type type){
        //        LocalVariable *&plvVariable = FindLocalVariable(strName);
        LocalVariable *&plvVariable = _mapLocalVariables[strName];
        if(plvVariable != null)
            return null;

        plvVariable = new LocalVariable(type, strName);
        AllocateLocalVariable *palv = new AllocateLocalVariable(plvVariable);
        Push(palv);

        return plvVariable;
    }

    LocalVariable *PushLocalVariable(const std::string &strName, Type type, Object *pValue){
        //        LocalVariable *&plvVariable = FindLocalVariable(strName);
        LocalVariable *&plvVariable = _mapLocalVariables[strName];
        if(plvVariable != null){
            delete pValue;
            return null;
        }

        plvVariable = new LocalVariable(type, strName);
        AllocateLocalVariable *palv = new AllocateLocalVariable(plvVariable);
        Push(palv);

        PushWriteLocalVariable(plvVariable, pValue);
        return plvVariable;
    }

    void PushWriteLocalVariable(LocalVariable *plvVariable, Object *pValue){
        WriteLocalVariable *pwlv = new WriteLocalVariable(plvVariable, pValue);
        Push(pwlv);
    }

    Object *NewAddInteger(Object *pLeft, Object *pRight){
        Binary *pAdd = new Binary(
                    Object::Type::otAddInteger,
                    pLeft, pRight);
        return pAdd;
    }

    Object *NewSubInteger(Object *pLeft, Object *pRight){
        Binary *pSub = new Binary(
                    Object::Type::otSubInteger,
                    pLeft, pRight);
        return pSub;
    }

    Object *NewLessOrEqualInteger(Object *pLeft, Object *pRight){
        Binary *pLessOrEqual = new Binary(
                    Object::Type::otLessOrEqualInteger,
                    pLeft, pRight);
        return pLessOrEqual;
    }

//    void PushReturn(Object *pValue){
////        Return *pReturn = new Return(pValue);
//        Return *pReturn = new Return();
//        pReturn->_block.Push(pValue)
//        Push(pReturn);
//    }

//    void PushIf(Object *pCondition, Statement *pTrue, Block *pFalseBlock){
//        If *pIf = new If(pCondition, pTrue, pFalseBlock);
//        Push(pIf);
//    }
//    void PushIf(Object *pCondition){
//        If *pIf = new If(pCondition);
//        Push(pIf);
//    }

    Call *NewCall(Function *pFunction, const Objects &inputs, const Objects &outputs){
        Call *pCall = new Call(pFunction, inputs, outputs);
        return pCall;
    }

    Bool Push(Statement *pStatement){
        if(_bReturn)
            return false;
        if(pStatement->getType() == Type::otReturn)
            _bReturn = true;
        _lstStatements.push_back(pStatement);
        return true;
    }

//    void PushFront(Object *pObject){
//        _lstStatements.push_front(pObject);
//    }

    std::list<Statement*> _lstStatements;
    const std::string &getName()const{
        return _strName;
    }

    void setName(const std::string &strName){
        _strName = strName;
    }

    Bool HasReturn()const{
        return _bReturn;
    }
private:
    //    LocalVariable *FindLocalVariable(const std::string &strName){
    //        LocalVariable *plvVariable = _mapLocalVariables[strName];
    //        if(plvVariable != null)
    //            return plvVariable;

    //        if(_pParent == null)
    //            return null;

    //        return _pParent->FindLocalVariable(strName);
    //    }

    std::string _strName;
    Block *_pParent = null;
    //    std::list<Call*> _lstCalls;
    Bool _bReturn = false;
    std::map<std::string, LocalVariable*> _mapLocalVariables;
};

}}//namespace elf.ast

#endif // BLOCK

