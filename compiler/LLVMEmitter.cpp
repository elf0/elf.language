//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//Optimize
#include <llvm/Analysis/Passes.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/IR/Verifier.h>

#include "emit/LLVMEmitter.h"
#include "emit/LLVMFunction.h"
#include "emit/LLVMStatement.h"
#include "emit/LLVMBlock.h"
#include "emit/LLVMCall.h"
#include "emit/LLVMIf.h"
#include "emit/LLVMBinary.h"
#include "emit/LLVMObject.h"
#include "emit/LLVMType.h"
#include "emit/LLVMUnsigned.h"
#include "emit/LLVMInteger.h"
#include "emit/LLVMFloat.h"
#include "emit/LLVMLocalVariable.h"

//#include "LLVMUnsigned.h"
//#include "LLVMBinary.h"
namespace elf{

LLVMEmitter::LLVMEmitter()
    : _llvmModule("elf", _llvmContext)
    , _llvmfpmOpt(&_llvmModule)
{
    _llvmfpmOpt.add(new llvm::DataLayoutPass(&_llvmModule));
    // Provide basic AliasAnalysis support for GVN.
    _llvmfpmOpt.add(llvm::createBasicAliasAnalysisPass());
    // Promote allocas to registers.
    _llvmfpmOpt.add(llvm::createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    _llvmfpmOpt.add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    _llvmfpmOpt.add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    _llvmfpmOpt.add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    _llvmfpmOpt.add(llvm::createCFGSimplificationPass());


//    _llvmfpmOpt.add(llvm::createLCSSAPass());

    _llvmfpmOpt.doInitialization();
}

llvm::Module *LLVMEmitter::Emit(ast::Module *pModule){
    _pModule = pModule;
    //    llvm::Module* pllvmModule = new llvm::Module(pModule->getName(), _llvmContext);
    for(auto pr: pModule->_mapObjects){
        //        ast::Function *pFunction = pr.second;
        ast::Object *pObject = pr.second;
        Emit(pObject);
    }

    //    llvm::errs() << "verifying... ";
    if (verifyModule(_llvmModule)) {
        //        llvm::errs() << argv[0] << ": Error constructing module!\n";
        return null;
    }
    return &_llvmModule;
}

llvm::Function *LLVMEmitter::Emit(ast::Function *pFunction){
    //    _pFunction = pFunction;
    //    _pBlock = pFunction->getEntryBlock();
    llvm::Type *pltReturn;
    U8 nInputs = pFunction->getInputCount();
    U8 nOutputs = pFunction->getOutputCount();
    U8 nllvmOutputs = nOutputs;
    if(nOutputs == 0)
        pltReturn = llvm::Type::getVoidTy(_llvmContext);
    else{
        ast::Argument &output0 = pFunction->getOutput(0);
        pltReturn = Emit(output0.getArgumentType());
        //        output0.setUserData(pltReturn);
        --nllvmOutputs;
    }

    U32 nArgs = nInputs + nllvmOutputs;
    //    _szArguments

    std::vector<llvm::Type*> parameters(nArgs);

    U8 i;
    for(i = 0; i < nInputs; ++i){
        ast::Argument &arg = pFunction->getInput(i);
        llvm::Type *pType = Emit(arg.getArgumentType());
        //        arg.setUserData(pType);
        parameters[i] = pType;
    }

    U32 li = nInputs;
    for(i = 1; i < nOutputs; ++i, ++li){
        ast::Argument &arg = pFunction->getOutput(i);
        llvm::Type *pType = Emit(arg.getArgumentType());
        //        arg.setUserData(pType);
        //FIXME: Output must be reference
        //            parameters[li] = Emit(arg.getArgumentType())->getPointerTo();
        parameters[li] = pType;
    }


    llvm::FunctionType *pftType = llvm::FunctionType::get(pltReturn, parameters, false);
    llvm::Function::LinkageTypes lltType = pFunction->_bExport? llvm::Function::ExternalLinkage : llvm::Function::PrivateLinkage;
    llvm::Function *pllvmFunction = llvm::Function::Create(pftType, lltType, pFunction->getName(), &_llvmModule);
    _pllvmFunction = pllvmFunction;
    //    pFunction->setUserData(pllvmFunction);

    llvm::Function::ArgumentListType::iterator iter = pllvmFunction->getArgumentList().begin();
    i = 0;
    while(i < nInputs){
        iter->setName(pFunction->getInputName(i));
        ast::Argument &arg = pFunction->getInput(i);
        arg.setUserData(iter);
        ++i;
        ++iter;
    }

    i = 1;
    while(i < nOutputs){
        iter->setName(pFunction->getOutputName(i));
        ast::Argument &arg = pFunction->getOutput(i);
        arg.setUserData(iter);
        ++i;
        ++iter;
    }

    Emit(pFunction->getEntryBlock());

    _llvmfpmOpt.run(*pllvmFunction);

    llvm::verifyFunction(*pllvmFunction);

    return pllvmFunction;
}

llvm::BasicBlock *LLVMEmitter::Emit(ast::Block *pBlock){
    //    ast::Block *pParentBlock = _pBlock;
    //    _pBlock = pBlock;

    llvm::BasicBlock *pllvmBlock = NewBlock(pBlock->getName());
    //    pBlock->setUserData(pllvmBlock);
    for(ast::Statement *pStatement: pBlock->_lstStatements){
        Emit(pStatement, _pllvmBlock);
    }

    //    _pBlock = pParentBlock;
    return pllvmBlock;
}

llvm::Value *LLVMEmitter::Emit(ast::Call *pCall, llvm::BasicBlock *pllvmBlock){
    llvm::Function *pllvmFunction = _pllvmFunction;
    U32 nArgs = pllvmFunction->getArgumentList().size();
    std::vector<llvm::Value *> vArgs(nArgs);
    EmitCallArguments(pCall->_inputs, pCall->_outputs, vArgs, pllvmBlock);

    //    int i = 0;
    //    assert((i >= pllvmFunction->getFunctionType()->getNumParams() ||
    //            pllvmFunction->getFunctionType()->getParamType(i) == args[i]->getType()) &&
    //           "Calling a function with a bad signature!");
    //    llvm::FunctionType *pftType = pllvmFunction->getFunctionType();
    //    int nP = pftType->getNumParams();
    //    llvm::Type *pFPT = pftType->getParamType(i);
    //    llvm::Type *pAT = args[i]->getType();
    //    assert(pFPT == pAT &&
    //           "Calling a function with a bad signature!");

    llvm::Value *pllvmResult = llvm::CallInst::Create(pllvmFunction, vArgs, "call", pllvmBlock);
    //    //FIXME: Need this?
    //            for(llvm::Value *pValue: vArgs)
    //                delete pValue;
    return pllvmResult;
    ////    llvm::Instruction::BinaryOps bin;
    ////    llvm::CmpInst::Predicate bin;
    ////    llvm::Value *pllvmFunction = null;
    //    llvm::Value *pllvmValue = Emit(pCall->_inputs[0]);
    ////    llvm::Value *pllvmRight = null;
    //    llvm::Value *pllvmRight = (llvm::Value*)Emit(pCall->_inputs[1]);
    //    Value::Type vtResultType = pCall->_outputs[0]._type;
    //    //    Value::Type vtResultType = pCall->_inputs[0]._type;
    //    llvm::Type llvmType = Emit(vtResultType);

    //    switch(pCall->_ftType){
    //    case Function::Type::ftExtendUnsigned:
    //        return new ZExtInst(pllvmValue, pllvmRight, "Unsigned.Extend", pllvmBlock);
    //    case Function::Type::ftTruncateUnsigned:
    //        return new TruncInst(pllvmValue, pllvmRight, "Unsigned.Truncate", pllvmBlock);
    //    case Function::Type::ftExtendInteger:
    //        return new SExtInst(pllvmValue, pllvmRight, "Integer.Extend", pllvmBlock);
    //    case Function::Type::ftTruncateInteger:
    //        return new TruncInst(pllvmValue, pllvmRight, "Integer.Truncate", pllvmBlock);
    //    case Function::Type::ftExtendFloat:
    //        return new FPExtInst(pllvmValue, pllvmRight, "Float.Extend", pllvmBlock);
    //    case Function::Type::ftTruncateFloat:
    //        return new FPTruncInst(pllvmValue, pllvmRight, "Float.Truncate", pllvmBlock);
    //    case Function::Type::ftUnsignedToFloat:
    //        return new UIToFPInst(pllvmValue, pllvmRight, "Unsigned.ToFloat", pllvmBlock);
    //    case Function::Type::ftFloatToUnsigned:
    //        return new FPToUIInst(pllvmValue, pllvmRight, "Float.ToUnsigned", pllvmBlock);
    //    case Function::Type::ftIntegerToFloat:
    //        return new SIToFPInst(pllvmValue, pllvmRight, "Integer.ToFloat", pllvmBlock);
    //    case Function::Type::ftFloatToInteger:
    //        return new FPToSIInst(pllvmValue, pllvmRight, "Float.ToInteger", pllvmBlock);
    //    case Function::Type::ftAddUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::Add, pllvmValue, pllvmRight, llvmType, "Unsigned.Add", pllvmBlock);
    //    case Function::Type::ftAddInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::Add, pllvmValue, pllvmRight, llvmType, "Integer.Add", pllvmBlock);
    //    case Function::Type::ftAddFloat:
    //        return new llvm::BinaryOperator(llvm::Instruction::FAdd, pllvmValue, pllvmRight, llvmType, "Float.Add", pllvmBlock);
    //    case Function::Type::ftSubUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::Sub, pllvmValue, pllvmRight, llvmType, "Unsigned.Sub", pllvmBlock);
    //    case Function::Type::ftSubInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::Sub, pllvmValue, pllvmRight, llvmType, "Integer.Sub", pllvmBlock);
    //    case Function::Type::ftSubFloat:
    //        return new llvm::BinaryOperator(llvm::Instruction::FSub, pllvmValue, pllvmRight, llvmType, "Float.Sub", pllvmBlock);
    //    case Function::Type::ftMulUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::Mul, pllvmValue, pllvmRight, llvmType, "Unsigned.Mul", pllvmBlock);
    //    case Function::Type::ftMulInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::Mul, pllvmValue, pllvmRight, llvmType, "Integer.Mul", pllvmBlock);
    //    case Function::Type::ftMulFloat:
    //        return new llvm::BinaryOperator(llvm::Instruction::FMul, pllvmValue, pllvmRight, llvmType, "Float.Mul", pllvmBlock);
    //    case Function::Type::ftDivUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::UDiv, pllvmValue, pllvmRight, llvmType, "Unsigned.Add", pllvmBlock);
    //    case Function::Type::ftDivInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::SDiv, pllvmValue, pllvmRight, llvmType, "Integer.Add", pllvmBlock);
    //    case Function::Type::ftDivFloat:
    //        return new llvm::BinaryOperator(llvm::Instruction::FDiv, pllvmValue, pllvmRight, llvmType, "Float.Add", pllvmBlock);
    //    case Function::Type::ftModUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::URem, pllvmValue, pllvmRight, llvmType, "Unsigned.Mod", pllvmBlock);
    //    case Function::Type::ftModInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::SRem, pllvmValue, pllvmRight, llvmType, "Integer.Mod", pllvmBlock);
    //    case Function::Type::ftModFloat:
    //        return new llvm::BinaryOperator(llvm::Instruction::FRem, pllvmValue, pllvmRight, llvmType, "Float.Mod", pllvmBlock);
    //    case Function::Type::ftShiftLeftUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::Shl, pllvmValue, pllvmRight, llvmType, "Unsigned.Shift.Left", pllvmBlock);
    //    case Function::Type::ftShiftLeftInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::Shl, pllvmValue, pllvmRight, llvmType, "Integer.Shift.Left", pllvmBlock);
    //    case Function::Type::ftShiftRightUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::LShr, pllvmValue, pllvmRight, llvmType, "Unsigned.Shift.Right", pllvmBlock);
    //    case Function::Type::ftShiftRightInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::LShr, pllvmValue, pllvmRight, llvmType, "Integer.Shift.Right", pllvmBlock);
    //    case Function::Type::ftAndUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::And, pllvmValue, pllvmRight, llvmType, "Unsigned.And", pllvmBlock);
    //    case Function::Type::ftAndInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::And, pllvmValue, pllvmRight, llvmType, "Integer.And", pllvmBlock);
    //    case Function::Type::ftOrUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::Or, pllvmValue, pllvmRight, llvmType, "Unsigned.Or", pllvmBlock);
    //    case Function::Type::ftOrInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::Or, pllvmValue, pllvmRight, llvmType, "Integer.Or", pllvmBlock);
    //    case Function::Type::ftNot:{
    //        //        Value *pZero = ConstantFP::getZeroValueForNegation(pType);
    ////        return new llvm::BinaryOperator(llvm::Instruction::Sub, pZero, pValue, llvmType, "Not", InsertAtEnd);
    //        //        return new llvm::BinaryOperator(llvm::Instruction::Sub, pValue, pllvmRight, llvmType, "Not", InsertAtEnd);
    //    }
    //    case Function::Type::ftXorUnsigned:
    //        return new llvm::BinaryOperator(llvm::Instruction::Xor, pllvmValue, pllvmRight, llvmType, "Unsigned.Xor", pllvmBlock);
    //    case Function::Type::ftXorInteger:
    //        return new llvm::BinaryOperator(llvm::Instruction::Xor, pllvmValue, pllvmRight, llvmType, "Integer.Xor", pllvmBlock);
    //    case Function::Type::ftEqualUnsigned:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_EQ, pllvmValue, pllvmRight, "Unsigned.Equal");
    //    case Function::Type::ftEqualInteger:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_EQ, pllvmValue, pllvmRight, "Integer.Equal");
    //    case Function::Type::ftEqual:
    //        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OEQ, pllvmValue, pllvmRight, "Float.Equal");
    //    case Function::Type::ftNotEqaulUnsigned:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_NE, pllvmValue, pllvmRight, "Unsigned.NotEqual");
    //    case Function::Type::ftNotEqaul:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_NE, pllvmValue, pllvmRight, "Integer.NotEqual");
    //    case Function::Type::ftNotEqaul:
    //        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_ONE, pllvmValue, pllvmRight, "Float.NotEqual");
    //    case Function::Type::ftLessUnsigned:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_ULT, pllvmValue, pllvmRight, "Unsigned.Less");
    //    case Function::Type::ftLessInteger:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SLT, pllvmValue, pllvmRight, "Integer.Less");
    //    case Function::Type::ftLessFloat:
    //        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OLT, pllvmValue, pllvmRight, "Float.Less");
    //    case Function::Type::ftLessOrEqaulUnsigned:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_ULE, pllvmValue, pllvmRight, "Unsigned.LessOrEqual");
    //    case Function::Type::ftLessOrEqaulInteger:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SLE, pllvmValue, pllvmRight, "Integer.LessOrEqual");
    //    case Function::Type::ftLessOrEqaulFloat:
    //        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OLE, pllvmValue, pllvmRight, "Float.LessOrEqual");
    //    case Function::Type::ftGreaterUnsigned:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_UGT, pllvmValue, pllvmRight, "Unsigned.Greater");
    //    case Function::Type::ftGreaterInteger:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SGT, pllvmValue, pllvmRight, "Integer.Greater");
    //    case Function::Type::ftGreaterFloat:
    //        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OGT, pllvmValue, pllvmRight, "Float.Greater");
    //    case Function::Type::ftGreaterOrEqaulUnsigned:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_UGE, pllvmValue, pllvmRight, "Unsigned.GreaterOrEqual");
    //    case Function::Type::ftGreaterOrEqaulInteger:
    //        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SGE, pllvmValue, pllvmRight, "Integer.GreaterOrEqual");
    //    case Function::Type::ftGreaterOrEqaulFloat:
    //        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OGE, pllvmValue, pllvmRight, "Float.GreaterOrEqual");
    //    case Function::Type::Custom:{
    //        llvm::ArrayRef<llvm::Value *> args = EmitCallArguments(pCall->_inputs, pCall->_outputs);
    //        llvm::Value *pllvmResult = llvm::CallInst::Create(pllvmValue, args, "call", pllvmBlock);
    //        //FIXME: Need this?
    ////        for(llvm::Value *pValue: args)
    ////            delete pValue;
    //        delete args.data();
    //        return pllvmResult;
    //        //        return llvm::CallInst::Create(Emit(pCall->_pFunction), Emit(pCall->_inputs, pCall->_outputs), "call", pllvmBlock);
    //    }
    //    }
}

void LLVMEmitter::EmitCallArguments(ast::Objects &inputs, ast::Objects &outputs, std::vector<llvm::Value *> &args, llvm::BasicBlock *pllvmBlock){
    U32 nInputs = inputs.getCount();
    U32 nOutputs = outputs.getCount();
    U32 i;
    for(i = 0; i < nInputs; ++i){
        args[i] = (llvm::Value*)Emit(inputs[i], pllvmBlock);
    }

    U32 li = nInputs;
    for(i = 1; i < nOutputs; ++i, ++li){
        args[li] = (llvm::Value*)Emit(outputs[i], pllvmBlock);
    }
}

llvm::Value *LLVMEmitter::Emit(ast::If *pIf, llvm::BasicBlock *pllvmBlock){
    llvm::Value *pLLVMCodition = (llvm::Value*)Emit(pIf->_pCondition, pllvmBlock);
    llvm::BasicBlock *pllvmTrueBlock = Emit(&pIf->_true);
    llvm::BasicBlock *pllvmFalseBlock = Emit(&pIf->_false);

    if(!pIf->_true.HasReturn()){
        llvm::BasicBlock *pllvmIfEndBlock = NewBlock("block.if.end");
        llvm::BranchInst::Create(pllvmIfEndBlock, pllvmTrueBlock);
    }
    //    ast::Statement *pTrue = pIf->_pTrue;
    //    Emit(((ast::Return*)pTrue), pllvmTrueBlock);
    //    ast::Statement *pFalse = pIf->_pFalse;
    //    if(pFalse != null)
    //        Emit(pFalse, pllvmFalseBlock);

    //    //    Bool bReturn = false;
    //    llvm::BasicBlock *pllvmIfEndBlock = null;
    //    switch(pTrue->getType()){
    //    case ast::Object::Type::otReturn:{
    //        //        pllvmIfEndBlock = NewBlock("block.if.end");
    //        Emit(((ast::Return*)pTrue), pllvmTrueBlock);
    //    }break;
    //    case ast::Object::Type::otBlock:{
    //        ast::Block *pBlock = (ast::Block*)pTrue;
    //        for(ast::Statement *pStatement: pBlock->_lstStatements){
    //            Emit(pStatement, pllvmTrueBlock);

    //            //FIXME: Return in substatement
    //            if(pStatement->getType() == ast::Object::Type::otReturn){
    //                pllvmIfEndBlock = NewBlock("block.if.end");
    //                break;
    //            }
    //        }
    //    }break;
    //    default:
    //        Emit(pTrue, pllvmTrueBlock);
    //        break;
    //    }

    //    if(pllvmIfEndBlock != null){
    //        llvm::BranchInst::Create(pllvmIfEndBlock, pllvmTrueBlock);
    //    }

    //    if(pIf->_pFalse != null){
    //        llvm::BasicBlock *pllvmFalseBlock = NewBlock("block.if.false", pllvmIfEndBlock);
    //        ast::Statement *pFalse = pIf->_pFalse;
    //        switch(pFalse->getType()){
    //        case ast::Object::Type::otReturn:{
    //            bReturn = true;
    //            Emit(((ast::Return*)pFalse), pllvmFalseBlock);
    //        }break;
    //        case ast::Object::Type::otBlock:{
    //            ast::Block *pBlock = (ast::Block*)pFalse;
    //            for(ast::Statement *pStatement: pBlock->_lstStatements){
    //                Emit(pStatement, pllvmFalseBlock);

    //                //FIXME: Return in substatement
    //                if(pStatement->getType() == ast::Object::Type::otReturn){
    //                    bReturn = true;
    //                    break;
    //                }
    //            }
    //        }break;
    //        default:
    //            Emit(pFalse, pllvmFalseBlock);
    //            break;
    //        }
    //    }



    //    llvm::Value *pllvmFalse = Emit(pIf->_pFalse);
    //    //    llvm::BasicBlock *pllvmFalseBlock;
    //    if(pFalse->getType() != ast::Object::Type::otBlock)
    //        pllvmFalseBlock = NewBlock("block.if.end");
    //    else
    //        pllvmFalseBlock = (llvm::BasicBlock*)Emit(pIf->_pFalse);
    //    //    llvm::BasicBlock *pllvmFalseBlock = NewBlock("block.false");
    //    //    switch(pFalse->getType()){
    //    //    case ast::Object::Type::otReturn:{
    //    //        ast::Return *pReturn = (ast::Return*)pFalse;
    //    ////        pllvmFalseBlock = NewBlock("block.false.return");
    //    //        //        pllvmTrueBlock = EmitBlock("block.true.return");
    //    //        llvm::ReturnInst::Create(_llvmContext, (llvm::Value*)Emit(pReturn->_pObject), pllvmFalseBlock);
    //    //        //        return Emit(((ast::Return*)pTrue));
    //    //    }break;
    //    ////    case ast::Object::Type::otBlock:{
    //    ////        ast::Block *pBlock = (ast::Block*)pFalse;
    //    ////        pllvmFalseBlock = Emit(pBlock);
    //    ////        //        pBlock->setUserData(pllvmTrueBlock);
    //    ////        //        _pBlock = pBlock;
    //    ////        //        if(pBlock->_lstStatements.size() > 0){
    //    ////        //            for(ast::Statement *pStatement: pBlock->_lstStatements){
    //    ////        //                Emit(pStatement);
    //    ////        //            }

    //    ////        //            if(pBlock->_lstStatements.back()->getType() == ast::Object::Type::otReturn)
    //    ////        //                break;
    //    ////        //        }
    //    ////        //        llvm::BranchInst::Create(pllvmIfEndBlock, pllvmTrueBlock);
    //    ////    }break;
    //    //    default:
    //    //        pllvmFalseBlock = NewBlock("block.false");
    //    //        Emit(pFalse);
    //    //        llvm::BasicBlock *pllvmIfEndBlock = NewBlock("block.if.end");
    //    //        llvm::BranchInst::Create(pllvmIfEndBlock, pllvmFalseBlock);
    //    //        break;
    //    //    }

    llvm::BranchInst *pResult = EmitIf(pLLVMCodition, pllvmTrueBlock, pllvmFalseBlock, pllvmBlock);
    return pResult;
}

llvm::BranchInst *LLVMEmitter::EmitIf(llvm::Value *pCondition, llvm::BasicBlock *pTrue, llvm::BasicBlock *pFalse, llvm::BasicBlock *pllvmBlock){
    return  llvm::BranchInst::Create(pTrue, pFalse, pCondition, pllvmBlock);
}

llvm::Value *LLVMEmitter::Emit(ast::InputReference *pRef){
    llvm::Value *pResult = (llvm::Value*)pRef->getArgument()->getUserData();
    return pResult;
}

//   void SetCurrentModule(llvm::Module *pModule){
//        _pModule = pModule;
//    }

llvm::Value *LLVMEmitter::Emit(ast::Return *pReturn, llvm::BasicBlock *pllvmBlock){
    llvm::Value *pllvmValue = (llvm::Value*)Emit(pReturn->_pObject, pllvmBlock);
    return EmitReturn(pllvmValue, pllvmBlock);
}

llvm::Value *LLVMEmitter::EmitReturn(llvm::Value *pResult, llvm::BasicBlock *pllvmBlock){
    return llvm::ReturnInst::Create(_llvmContext, pResult, pllvmBlock);
}

}//namespace elf
