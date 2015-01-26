#ifndef LLVMUNSIGNED
#define LLVMUNSIGNED

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Unsigned.h"

namespace elf{

//class LLVMUnsigned: public ast::Unsigned{
//public:
//private:
//};

//llvm::Value *LLVMEmitter::Emit(ast::Unsigned *pValue){
//    llvm::Value *pResult;
//    switch(pValue->getType()){
//    case ast::Object::Type::otUnsigned8:
//        pResult = llvm::ConstantInt::get(llvm::Type::getInt8Ty(_llvmContext), pValue->_u8Value, false);
//        break;
//    case ast::Object::Type::otUnsigned16:
//        pResult = llvm::ConstantInt::get(llvm::Type::getInt16Ty(_llvmContext), pValue->_u16Value, false);
//        break;
//    case ast::Object::Type::otUnsigned32:
//        pResult = llvm::ConstantInt::get(llvm::Type::getInt32Ty(_llvmContext), pValue->_u32Value, false);
//        break;
//    case ast::Object::Type::otUnsigned64:
//        pResult = llvm::ConstantInt::get(llvm::Type::getInt64Ty(_llvmContext), pValue->_u64Value, false);
//        break;
//    }
//    return pResult;
//}

//static inline llvm::Value *LLVMEmitter::Emit(Value::Type type, Unsigned *pLeft, Unsigned *pRight, llvm::Value **ppResultRight) {
//    llvm::Value *pResultLeft;
//    llvm::Value *pResultRight;
//    switch(type){
//    case Value::Type::Unsigned8:
//        pResultLeft = llvm::ConstantInt::get(llvm::Type::getInt8Ty(*pContext), pLeft->_u8Value, false);
//        pResultRight = llvm::ConstantInt::get(llvm::Type::getInt8Ty(*pContext), pRight->_u8Value, false);
//        break;
//    case Value::Type::Unsigned16:
//        pResultLeft = llvm::ConstantInt::get(llvm::Type::getInt16Ty(*pContext), pLeft->_u16Value, false);
//        pResultRight = llvm::ConstantInt::get(llvm::Type::getInt16Ty(*pContext), pRight->_u16Value, false);
//        break;
//    case Value::Type::Unsigned32:
//        pResultLeft = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*pContext), pLeft->_u32Value, false);
//        pResultRight = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*pContext), pRight->_u32Value, false);
//        break;
//    case Value::Type::Unsigned64:
//        pResultLeft = llvm::ConstantInt::get(llvm::Type::getInt64Ty(*pContext), pLeft->_u64Value, false);
//        pResultRight = llvm::ConstantInt::get(llvm::Type::getInt64Ty(*pContext), pRight->_u64Value, false);
//        break;
//    }

//    *ppResultRight = pResultRight;
//    return pResultLeft;
//}

//// Both pLeft and pRight must have identical types, but DO NOT check it here.
//static inline llvm::Value *LLVMEmitter::EmitBinary(Unsigned::Binary op, Unsigned *pLeft, Unsigned *pRight){
////    llvm::Value *pLLVMRight;
////    llvm::Value *pLLVMLeft = Unsigned2_ToLLVM2(pLeft->_type, pLeft, pRight, &pLLVMRight);
//    switch (pBinary->getType()){
//    case ast::Object::Type::otvtExtendUnsigned:
//        return new llvm::ZExtInst(pllvmLeft, pllvmType, "Unsigned.Extend", _pBlock);
//    case ast::Object::Type::otvtTruncateUnsigned:
//        return new llvm::TruncInst(pllvmLeft, pllvmType, "Unsigned.Truncate", _pBlock);
//    case ast::Object::Type::otvtUnsignedToFloat:
//        return new llvm::UIToFPInst(pllvmLeft, pllvmType, "Unsigned.ToFloat", _pBlock);
//    case ast::Object::Type::otvtAddUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::Add, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Add", _pBlock);
//    case ast::Object::Type::otvtSubUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::Sub, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Sub", _pBlock);
//    case ast::Object::Type::otvtMulUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::Mul, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Mul", _pBlock);
//    case ast::Object::Type::otvtDivUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::UDiv, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Div", _pBlock);
//    case ast::Object::Type::otvtModUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::URem, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Mod", _pBlock);
//    case ast::Object::Type::otvtShiftLeftUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::Shl, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Shift.Left", _pBlock);
//    case ast::Object::Type::otvtShiftRightUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::LShr, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Shift.Right", _pBlock);
//    case ast::Object::Type::otvtAndUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::And, pllvmLeft, pllvmRight, pllvmType, "Unsigned.And", _pBlock);
//    case ast::Object::Type::otvtOrUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::Or, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Or", _pBlock);
//    case ast::Object::Type::otvtXorUnsigned:
//        return new llvm::BinaryOperator(llvm::Instruction::Xor, pllvmLeft, pllvmRight, pllvmType, "Unsigned.Xor", _pBlock);
//    case ast::Object::Type::otvtEqualUnsigned:
//        return new llvm::ICmpInst(*_pBlock, llvm::ICmpInst::ICMP_EQ, pllvmLeft, pllvmRight, "Unsigned.Equal");
//    case ast::Object::Type::otvtNotEqualUnsigned:
//        return new llvm::ICmpInst(*_pBlock, llvm::ICmpInst::ICMP_NE, pllvmLeft, pllvmRight, "Unsigned.NotEqual");
//    case ast::Object::Type::otvtLessUnsigned:
//        return new llvm::ICmpInst(*_pBlock, llvm::ICmpInst::ICMP_ULT, pllvmLeft, pllvmRight, "Unsigned.Less");
//    case ast::Object::Type::otvtLessOrEqualUnsigned:
//        return new llvm::ICmpInst(*_pBlock, llvm::ICmpInst::ICMP_ULE, pllvmLeft, pllvmRight, "Unsigned.LessOrEqual");
//    case ast::Object::Type::otvtGreaterUnsigned:
//        return new llvm::ICmpInst(*_pBlock, llvm::ICmpInst::ICMP_UGT, pllvmLeft, pllvmRight, "Unsigned.Greater");
//    case ast::Object::Type::otvtGreaterOrEqualUnsigned:
//        return new llvm::ICmpInst(*_pBlock, llvm::ICmpInst::ICMP_UGE, pllvmLeft, pllvmRight, "Unsigned.GreaterOrEqual");
//    }
//    //error
//    return null;
//}

llvm::Value *LLVMEmitter::EmitBool(Bool value) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(_llvmContext), value, false);
}

llvm::Value *LLVMEmitter::EmitU8(U8 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(_llvmContext), value, false);
}

llvm::Value *LLVMEmitter::EmitU16(U16 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt16Ty(_llvmContext), value, false);
}

llvm::Value *LLVMEmitter::EmitU32(U32 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(_llvmContext), value, false);
}

llvm::Value *LLVMEmitter::EmitU64(U64 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(_llvmContext), value, false);
}

}//namespace elf

#endif // LLVMUNSIGNED

