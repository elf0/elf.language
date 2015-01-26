#ifndef OBJECT
#define OBJECT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include <set>
#include <string>
#include <map>
#include "Type.h"

namespace elf{namespace ast{

#ifdef DEBUG
class ObjectStatistic{
public:
    typedef std::map<std::string, U32> Container;
    typedef typename Container::iterator Iterator;
    typedef typename Container::const_iterator  ConstIterator;

    void New(const std::string &strClassName){
        U32 &nCount = _objects[strClassName];
        ++nCount;
    }

    void Delete(const std::string &strClassName){
        U32 &nCount = _objects[strClassName];
        --nCount;
    }

    Iterator begin(){
        return _objects.begin();
    }

    ConstIterator begin()const{
        return _objects.begin();
    }

    Iterator end(){
        return _objects.end();
    }

    ConstIterator end()const{
        return _objects.end();
    }
private:
    Container _objects;
};
#endif

class Object{
public:
    //    enum class Type{
    //        otUnknown, otReference,//otVariableReference, otArgumentReference,
    //        //struct, class..
    //        otCustom,
    //        otType, //pass type as a value
    //        otVoid, otConstant,
    //        otVariable, otArgument,
    //        otFunction,// otFunctionName,
    //        otNamespace,
    //        otBlock, otBoolean,
    //        //Constant
    //        otTrue, otFalse, otString,
    //        otUnsigned8, otUnsigned16, otUnsigned32, otUnsigned64,
    //        otInteger8, otInteger16, otInteger32, otInteger64,
    //        otFloat32, otFloat64,
    //        //Expression
    //        otDefine,
    //        otUnary, otBinary,
    //        otNotUnsigned, otNotInteger,

    //        //Convertion
    //        otExtendUnsigned, otTruncateUnsigned,
    //        otExtendInteger, otTruncateInteger,
    //        otExtendFloat, otTruncateFloat,
    //        otUnsignedToFloat, otFloatToUnsigned,
    //        otIntegerToFloat, otFloatToInteger,
    //        //Statement
    //        otDefineVariable, otDefineFunction, otIf, otReturn, otCall,
    //    };
    enum class Type{
        otUnknown, otName, otModule,
        otValue, otVariable, otLocalVariable, otArgument, otArgumentReference, otInputReference, otOutputReference, otExpression,
        otFunction, otFunctionType, otStructure, otClass, otNamespace,
        //Statement
        otStatement,
        otBlock, otAllocateLocalVariable, otReadLocalVariable, otWriteLocalVariable,
        otUnary, otTernary, otCall, otReturn, otIf, otLoot, otSwitch,
        //Constant
        ConstantInteger, ConstantFloat, ConstantString,

        //        otBoolean, //8 bits
        //        otUnsigned, //64 bits
        //        otInteger, //64 bits
        //        otFloat, //64 bits
        //        otString, //std::string
        otConstantBoolean,
        otConstantUnsigned8, otConstantUnsigned16, otConstantUnsigned32, otConstantUnsigned64,
        otConstantInteger8, otConstantInteger16, otConstantInteger32, otConstantInteger64,
        otConstantFloat32, otConstantFloat64,

        //otVariable,
        otBoolean,
        otUnsigned8, otUnsigned16, otUnsigned32, otUnsigned64,
        otInteger8, otInteger16, otInteger32, otInteger64,
        otFloat32, otFloat64,
        otReference,

        //
        otExtendUnsigned, otTruncateUnsigned,
        otExtendInteger, otTruncateInteger,
        otExtendFloat, otTruncateFloat,
        otUnsignedToFloat, otFloatToUnsigned,
        otIntegerToFloat, otFloatToInteger,
        //Binary
        //, otBinary
        otAddUnsigned, otAddInteger, otAddFloat,
        otSubUnsigned, otSubInteger, otSubFloat,
        otMulUnsigned, otMulInteger, otMulFloat,
        otDivUnsigned, otDivInteger, otDivFloat,
        otModUnsigned, otModInteger, otModFloat,
        otShiftLeftUnsigned, otShiftLeftInteger,
        otShiftRightUnsigned, otShiftRightInteger,
        otAndUnsigned, otAndInteger,
        otOrUnsigned, otOrInteger,
        otXorUnsigned, otXorInteger,
        otEqualUnsigned, otEqualInteger, otEqualFloat,
        otNotEqualUnsigned, otNotEqualInteger, otNotEqualFloat,
        otLessUnsigned, otLessInteger, otLessFloat,
        otLessOrEqualUnsigned, otLessOrEqualInteger, otLessOrEqualFloat,
        otGreaterUnsigned, otGreaterInteger, otGreaterFloat,
        otGreaterOrEqualUnsigned, otGreaterOrEqualInteger, otGreaterOrEqualFloat
    };

    //    Object(){
    //        ++s_nObjects;
    //    }

    Object(Type type)
        : _type(type)
    {
#ifdef DEBUG
        ++s_nObjects;
#endif
    }

    virtual ~Object(){
#ifdef DEBUG
        --s_nObjects;
#endif
    }

#ifdef DEBUG
    static const ObjectStatistic &getObjectStatistic(){
        return s_osObjects;
    }

    static U32 getObjects(){
        return s_nObjects;
    }
#endif

    Type getType(){
        return _type;
    }

    void setType(Type type){
        _type = type;
    }

    void *getUserData()const{
        return _pUserData;
    }

    void setUserData(void *pData){
        _pUserData = pData;
    }
protected:
#ifdef DEBUG
    static ObjectStatistic s_osObjects;
#endif
private:
    void AddUser(Object *pObject){
        _setUsers.insert(pObject);
    }

    void RemoveUser(Object *pObject){
        _setUsers.erase(pObject);
    }

    Type _type = Type::otUnknown;
    //    Object *_pTypeObject;
    void *_pUserData = null;
    std::set<Object*> _setUsers;

    //
#ifdef DEBUG
    static U32 s_nObjects;
#endif
};

class Objects{
public:
    Objects(U32 nCount)
        : _nCount(nCount)
        , _pszObjects(new Object*[nCount])
    {}

    Objects(const Objects &other)
        : _nCount(other._nCount)
        , _pszObjects(new Object*[_nCount])
    {
        for(U32 i = 0; i < _nCount; ++i)
            _pszObjects[i] = other._pszObjects[i];
    }

    ~Objects(){
        for(U32 i = 0; i < _nCount; ++i)
            delete _pszObjects[i];
        delete []_pszObjects;
    }

    Object* &operator[](U32 nIndex){
        return _pszObjects[nIndex];
    }

    U32 getCount()const{
        return _nCount;
    }

    void Clear(){
        for(U32 i = 0; i < _nCount; ++i)
            _pszObjects[i] = null;
    }

private:
    U32 _nCount = 0;
    Object** _pszObjects = null;
};

//class Type: public Object{
//public:
//};

//class Value: public Object{
//public:
//};

//class Reference: public Object{
//public:
//};

}}//namespace elf.ast

#endif // OBJECT

