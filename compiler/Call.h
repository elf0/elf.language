#ifndef CALL
#define CALL


//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//#include "Function.h"
#include "Statement.h"

namespace elf{namespace ast{

class Function;

class Call: public Statement{
public:
    Call(Function *pFunction, const Objects &inputs, const Objects &outputs)
        : Statement(Type::otCall)
        , _pFunction(pFunction)
        //        , _ftType(Function::Type::ftCustom)
        //        , _strFunction(strFunction)
        , _inputs(inputs)
        , _outputs(outputs)
    {
#ifdef DEBUG
        s_osObjects.New("Call");
#endif
    }
    //    Call(const std::string &strFunction, const Objects &inputs, const Objects &outputs)
    //        : Object(Type::otCall)
    //        , _ftType(Function::Type::ftCustom)
    //        , _strFunction(strFunction)
    //        , _inputs(inputs)
    //        , _outputs(outputs)
    //    {}

    //    Call(Function::Type ftType, const Objects &inputs, const Objects &outputs)
    //        : Object(Type::otCall)
    //        , _ftType(ftType)
    //        , _strFunction(strFunction)
    //        , _inputs(inputs)
    //        , _outputs(outputs)
    //    {}

    ~Call(){
        //        fprintf(stderr, "~Call.\"%s\"()\n", _pFunction->getName().c_str());
#ifdef DEBUG
        s_osObjects.Delete("Call");
        fprintf(stderr, "~Call(%p)\n", _pFunction);
#endif
    }

    //    Function::Type _ftFunction;
    //    std::string _strFunction;
    Function *_pFunction;
    //_intputs[0] = FunctionReference or FunctionName
    Objects _inputs;
    //_outputs[0] = ReturnType
    Objects _outputs;
};

}}//namespace elf.ast

#endif // CALL

