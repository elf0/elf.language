#ifndef FUNCTION
#define FUNCTION

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//#include <list>

#include "Block.h"
//#include "Expression.h"
#include "FunctionType.h"

namespace elf{namespace ast{

//class Function: public Value{
class Function: public Object{
public:
    //    enum class Type{
    //        //Function*(arg0, ..)
    //        ftCustom,
    //        //Unary(value)
    //        ftIncrease, ftDecrease, ftCast,
    //        //Binary(left, right)
    ////        ftAdd, ftSub, ftMul, ftDiv, ftMod,
    //        ftAddUnsigned, ftAddInteger, ftAddFloat,
    //        ftSubUnsigned, ftSubInteger, ftSubFloat,
    //        ftMulUnsigned, ftMulInteger, fftMulFloat,
    //        ftDivUnsigned, ftDivInteger, ftDivFloat,
    //        ftModUnsigned, ftModInteger, ftModFloat,
    //        ftShiftLeftUnsigned, ftShiftLeftInteger,
    //        ftShiftRightUnsigned, ftShiftRightInteger,
    //        ftAndUnsigned, ftAndInteger,
    //        ftOrUnsigned, ftOrInteger,
    //        tNotUnsigned, tNotInteger,
    //        tXorUnsigned, tXorInteger,
    //        ftEqualUnsigned, ftEqualInteger, ftEqualFloat,
    //        ftNotEqaulUnsigned, ftNotEqaulInteger, ftNotEqaulFloat,
    //        ftLessUnsigned, ftLessInteger, ftLess,
    //        ftLessOrEqaulUnsigned, ftLessOrEqaulInteger, ftLessOrEqaulFloat,
    //        ftGreaterUnsigned, ftGreaterInteger, ftGreater,
    //        ftGreaterOrEqaulUnsigned, ftGreaterOrEqaulInteger, ftGreaterOrEqaulFloat,
    //        //Convertion
    //        ftExtendUnsigned, ftTruncateUnsigned,
    //        ftExtendInteger, ftTruncateInteger,
    //        ftExtendFloat, ftTruncateFloat,
    //        ftUnsignedToFloat, ftFloatToUnsigned,
    //        ftIntegerToFloat, ftFloatToInteger,
    //        //If(Condition, True);
    //        //If(Condition, True, False);
    //        ftIf,
    //        //ForEach(Container, Function)
    //        ftForEach,
    //        ftReturn
    //    };

    Function(const std::string &strName, FunctionType *pftFunctiionType)
        : Object(Type::otFunction)
        , _strName(strName)
        , _pftFunctionType(pftFunctiionType)
        , _pszInputNames(new std::string[pftFunctiionType->getInputCount()])
        , _pszOutputNames(new std::string[pftFunctiionType->getOutputCount()])
        , _blkBody("block.entry")
    {}


    ~Function(){
        //        delete _pBlock;
        //        for(auto pr: _mapLocalVariables)
        //            delete pr.second;
        delete []_pszInputNames;
        delete []_pszOutputNames;
        delete _pftFunctionType;
#ifdef DEBUG
        fprintf(stderr, "~Function.\"%s\"()\n", _strName.c_str());
#endif
    }

#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "Function.new\n");
        s_osObjects.New("Function");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "Function.delete\n");
        s_osObjects.Delete("Function");
        ::operator delete(pObject);
    }
#endif

    const std::string &getName()const{
        return _strName;
    }

    U8 getInputCount()const{
        return _pftFunctionType->getInputCount();
    }

    U8 getOutputCount()const{
        return _pftFunctionType->getOutputCount();
    }

    Argument &getInput(U8 nIndex)const{
        return _pftFunctionType->getInput(nIndex);
    }

    Argument &getOutput(U8 nIndex)const{
        return _pftFunctionType->getOutput(nIndex);
    }

    U8 getInputIndex(const std::string &strName)const{
        for(U8 i = 0, n = getInputCount(); i < n; ++i){
            if(strName == _pszInputNames[i])
                return i;
        }
        return 0xFF;
    }

    U8 getOutputIndex(const std::string &strName)const{
        for(U8 i = 0, n = getOutputCount(); i < n; ++i){
            if(strName == _pszOutputNames[i])
                return i;
        }
        return 0xFF;
    }

    const std::string &getInputName(U8 nIndex)const{
        return _pszInputNames[nIndex];
    }

    const std::string &getOutputName(U8 nIndex)const{
        return _pszOutputNames[nIndex];
    }

    void setInputName(U8 nIndex, const std::string &strName){
        _pszInputNames[nIndex] = strName;
    }

    void setOutputName(U8 nIndex, const std::string &strName){
        _pszOutputNames[nIndex] = strName;
    }

    Block *getEntryBlock(){
        return &_blkBody;
    }

    const Block *getEntryBlock()const{
        return &_blkBody;
    }
    //    elf::ast::LocalVariable *NewLocalVariable(const std::string &strName, Type type){
    //        LocalVariable *&plvVariable = _mapLocalVariables[strName];
    //        if(plvVariable != null)
    //            return null;

    //        plvVariable = new elf::ast::LocalVariable(type, strName);
    //        AllocateLocalVariable *palv = new AllocateLocalVariable(plvVariable);
    //        _blkBody.PushFront(palv);
    //        return plvVariable;
    //    }

    //    Type _ftFunction = Type::ftCustom;
    bool _bExport = false;
private:
    FunctionType *_pftFunctionType;
    std::string _strName;
    std::string *_pszInputNames = null;
    std::string *_pszOutputNames = null;
    Block _blkBody;
    //    std::map<std::string, LocalVariable*> _mapLocalVariables;
};

}}//namespace elf.ast
#endif // FUNCTION

