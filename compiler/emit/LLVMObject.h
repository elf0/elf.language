#ifndef LLVMOBJECT
#define LLVMOBJECT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

//#include "Object.h"
#include "Integer.h"
#include "Float.h"
//#include "ArgumentReference.h"

namespace elf{

void *LLVMEmitter::Emit(ast::Object *pObject){
    switch(pObject->getType()){
    case ast::Object::Type::otFunction:
        return Emit((ast::Function*)pObject);
    case ast::Object::Type::otBlock:
        return Emit((ast::Block*)pObject);
    }
    //Error
    fprintf(stderr, "Error: Unexpected object!\n");
    return null;
}

void *LLVMEmitter::Emit(ast::Object *pObject, llvm::BasicBlock *pllvmBlock){
    switch(pObject->getType()){
    //Value
    case ast::Object::Type::otBoolean:
        return EmitBool(((ast::Unsigned*)pObject)->getU8());
    case ast::Object::Type::otUnsigned8:
        return EmitU8(((ast::Unsigned*)pObject)->getU8());
    case ast::Object::Type::otUnsigned16:
        return EmitU16(((ast::Unsigned*)pObject)->getU16());
    case ast::Object::Type::otUnsigned32:
        return EmitU32(((ast::Unsigned*)pObject)->getU32());
    case ast::Object::Type::otUnsigned64:
        return EmitU64(((ast::Unsigned*)pObject)->getU64());
    case ast::Object::Type::otInteger8:
        return EmitI8(((ast::Integer*)pObject)->getI8());
    case ast::Object::Type::otInteger16:
        return EmitI16(((ast::Integer*)pObject)->getI16());
    case ast::Object::Type::otInteger32:
        return EmitI32(((ast::Integer*)pObject)->getI32());
    case ast::Object::Type::otInteger64:
        return EmitI64(((ast::Integer*)pObject)->getI64());
    case ast::Object::Type::otFloat32:
        return EmitF32(((ast::Float*)pObject)->getValue32());
    case ast::Object::Type::otFloat64:
        return EmitF64(((ast::Float*)pObject)->getValue64());
        //Binary
        //FIXME: Type implemention
    case ast::Object::Type::otExtendUnsigned:
        return new llvm::ZExtInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Extend", pllvmBlock);
    case ast::Object::Type::otTruncateUnsigned:
        return new llvm::TruncInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Truncate", pllvmBlock);
    case ast::Object::Type::otExtendInteger:
        return new llvm::SExtInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Extend", pllvmBlock);
    case ast::Object::Type::otTruncateInteger:
        return new llvm::TruncInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Truncate", pllvmBlock);
    case ast::Object::Type::otExtendFloat:
        return new llvm::FPExtInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Extend", pllvmBlock);
    case ast::Object::Type::otTruncateFloat:
        return new llvm::FPTruncInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Truncate", pllvmBlock);
    case ast::Object::Type::otUnsignedToFloat:
        return new llvm::UIToFPInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.ToFloat", pllvmBlock);
    case ast::Object::Type::otFloatToUnsigned:
        return new llvm::FPToUIInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.ToUnsigned", pllvmBlock);
    case ast::Object::Type::otIntegerToFloat:
        return new llvm::SIToFPInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.ToFloat", pllvmBlock);
    case ast::Object::Type::otFloatToInteger:
        return new llvm::FPToSIInst((llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Type*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.ToInteger", pllvmBlock);
    case ast::Object::Type::otAddUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::Add, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Add", pllvmBlock);
    case ast::Object::Type::otAddInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::Add, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Add", pllvmBlock);
    case ast::Object::Type::otAddFloat:
        return llvm::BinaryOperator::Create(llvm::Instruction::FAdd, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Add", pllvmBlock);
    case ast::Object::Type::otSubUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::Sub, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Sub", pllvmBlock);
    case ast::Object::Type::otSubInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::Sub, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Sub", pllvmBlock);
    case ast::Object::Type::otSubFloat:
        return llvm::BinaryOperator::Create(llvm::Instruction::FSub, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Sub", pllvmBlock);
    case ast::Object::Type::otMulUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::Mul, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Mul", pllvmBlock);
    case ast::Object::Type::otMulInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::Mul, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Mul", pllvmBlock);
    case ast::Object::Type::otMulFloat:
        return llvm::BinaryOperator::Create(llvm::Instruction::FMul, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Mul", pllvmBlock);
    case ast::Object::Type::otDivUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::UDiv, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Add", pllvmBlock);
    case ast::Object::Type::otDivInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::SDiv, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Add", pllvmBlock);
    case ast::Object::Type::otDivFloat:
        return llvm::BinaryOperator::Create(llvm::Instruction::FDiv, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Add", pllvmBlock);
    case ast::Object::Type::otModUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::URem, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Mod", pllvmBlock);
    case ast::Object::Type::otModInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::SRem, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Mod", pllvmBlock);
    case ast::Object::Type::otModFloat:
        return llvm::BinaryOperator::Create(llvm::Instruction::FRem, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Mod", pllvmBlock);
    case ast::Object::Type::otShiftLeftUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::Shl, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Shift.Left", pllvmBlock);
    case ast::Object::Type::otShiftLeftInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::Shl, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Shift.Left", pllvmBlock);
    case ast::Object::Type::otShiftRightUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::LShr, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Shift.Right", pllvmBlock);
    case ast::Object::Type::otShiftRightInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::LShr, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Shift.Right", pllvmBlock);
    case ast::Object::Type::otAndUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::And, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.And", pllvmBlock);
    case ast::Object::Type::otAndInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::And, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.And", pllvmBlock);
    case ast::Object::Type::otOrUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::Or, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Or", pllvmBlock);
    case ast::Object::Type::otOrInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::Or, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Or", pllvmBlock);
    case ast::Object::Type::otXorUnsigned:
        return llvm::BinaryOperator::Create(llvm::Instruction::Xor, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Xor", pllvmBlock);
    case ast::Object::Type::otXorInteger:
        return llvm::BinaryOperator::Create(llvm::Instruction::Xor, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Xor", pllvmBlock);
    case ast::Object::Type::otEqualUnsigned:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_EQ, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Equal");
    case ast::Object::Type::otEqualInteger:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_EQ, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Equal");
    case ast::Object::Type::otEqualFloat:
        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OEQ, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Equal");
    case ast::Object::Type::otNotEqualUnsigned:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_NE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.NotEqual");
    case ast::Object::Type::otNotEqualInteger:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_NE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.NotEqual");
    case ast::Object::Type::otNotEqualFloat:
        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_ONE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.NotEqual");
    case ast::Object::Type::otLessUnsigned:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_ULT, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Less");
    case ast::Object::Type::otLessInteger:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SLT, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Less");
    case ast::Object::Type::otLessFloat:
        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OLT, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Less");
    case ast::Object::Type::otLessOrEqualUnsigned:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_ULE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.LessOrEqual");
    case ast::Object::Type::otLessOrEqualInteger:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SLE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.LessOrEqual");
    case ast::Object::Type::otLessOrEqualFloat:
        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OLE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.LessOrEqual");
    case ast::Object::Type::otGreaterUnsigned:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_UGT, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.Greater");
    case ast::Object::Type::otGreaterInteger:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SGT, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.Greater");
    case ast::Object::Type::otGreaterFloat:
        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OGT, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.Greater");
    case ast::Object::Type::otGreaterOrEqualUnsigned:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_UGE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Unsigned.GreaterOrEqual");
    case ast::Object::Type::otGreaterOrEqualInteger:
        return new llvm::ICmpInst(*pllvmBlock, llvm::ICmpInst::ICMP_SGE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Integer.GreaterOrEqual");
    case ast::Object::Type::otGreaterOrEqualFloat:
        return new llvm::FCmpInst(*pllvmBlock, llvm::FCmpInst::FCMP_OGE, (llvm::Value*)Emit(((ast::Binary*)pObject)->_pLeft, pllvmBlock), (llvm::Value*)Emit(((ast::Binary*)pObject)->_pRight, pllvmBlock), "Float.GreaterOrEqual");
        //    case ast::Object::Type::otType:
        //        return Emit(pValue->_ttValue);
//    case ast::Object::Type::otFunction:
//        return Emit((ast::Function*)pObject);
    case ast::Object::Type::otInputReference:
        return Emit((ast::InputReference*)pObject);
        //Statement

    case ast::Object::Type::otCall:
        return Emit((ast::Call*)pObject, pllvmBlock);
    case ast::Object::Type::otReturn:
        return Emit((ast::Return*)pObject, pllvmBlock);
    case ast::Object::Type::otIf:
        return Emit((ast::If*)pObject, pllvmBlock);
    case ast::Object::Type::otAllocateLocalVariable:
        return Emit((ast::AllocateLocalVariable*)pObject, pllvmBlock);
    case ast::Object::Type::otReadLocalVariable:
        return Emit((ast::ReadLocalVariable*)pObject, pllvmBlock);
    case ast::Object::Type::otWriteLocalVariable:
        return Emit((ast::WriteLocalVariable*)pObject, pllvmBlock);
      //    case ast::Object::Type::otNot:{
        //        //        Value *pZero = ConstantFP::getZeroValueForNegation(pType);
        ////        return new llvm::BinaryOperator(llvm::Instruction::Sub, pZero, pValue, pllvmType, "Not", InsertAtEnd);
        //        //        return new llvm::BinaryOperator(llvm::Instruction::Sub, pValue, pllvmRight, pllvmType, "Not", InsertAtEnd);
        //    }
    }

    //Error
    fprintf(stderr, "Error: Unexpected object!\n");
    return null;
}

}//namespace elf

#endif // LLVMOBJECT

