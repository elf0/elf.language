#ifndef FUNCTIONTYPE
#define FUNCTIONTYPE

#include "Argument.h"

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{namespace ast{

class FunctionType: public Object{
public:
    FunctionType(U8 nInputs, U8 nOutputs)
        : Object(Type::otFunctionType)
        , _inputs(nInputs)
        , _outputs(nOutputs)
    {}

    FunctionType(U8 nInputs, U8 nOutputs, Object::Type vtType)
        : Object(Type::otFunctionType)
        , _inputs(nInputs, vtType)
        , _outputs(nOutputs, vtType)
    {}

    FunctionType(U8 nInputs, Object::Type vtInput, U8 nOutputs, Object::Type vtOutput)
        : Object(Type::otFunction)
        , _inputs(nInputs, vtInput)
        , _outputs(nOutputs, vtOutput)
    {}

    ~FunctionType(){
    }
#ifdef DEBUG
    void* operator new(std::size_t nSize){
        fprintf(stderr, "FunctionType.new\n");
        s_osObjects.New("FunctionType");
        return ::operator new(nSize);
    }

    void operator delete(void *pObject){
        fprintf(stderr, "FunctionType.delete\n");
        s_osObjects.Delete("FunctionType");
        ::operator delete(pObject);
    }
#endif
    U8 getInputCount()const{
        return _inputs.getCount();
    }

    U8 getOutputCount()const{
        return _outputs.getCount();
    }

    Argument &getInput(U8 nIndex)const{
        return _inputs[nIndex];
    }

    Argument &getOutput(U8 nIndex)const{
        return _outputs[nIndex];
    }
private:
    Arguments _inputs;
    Arguments _outputs;
};

}}//namespace elf.ast

#endif // FUNCTIONTYPE

