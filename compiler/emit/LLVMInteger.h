#ifndef LLVMINTEGER
#define LLVMINTEGER

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{

llvm::Value *LLVMEmitter::EmitI8(I8 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(_llvmContext), value, true);
}

llvm::Value *LLVMEmitter::EmitI16(I16 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt16Ty(_llvmContext), value, true);
}

llvm::Value *LLVMEmitter::EmitI32(I32 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(_llvmContext), value, true);
}

llvm::Value *LLVMEmitter::EmitI64(I64 value) {
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(_llvmContext), value, true);
}

}//namespace elf

#endif // LLVMINTEGER

