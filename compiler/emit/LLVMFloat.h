#ifndef LLVMFLOAT
#define LLVMFLOAT

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{

llvm::Value *LLVMEmitter::EmitF32(F32 value) {
    return llvm::ConstantFP::get(llvm::Type::getFloatTy(_llvmContext), value);
}

llvm::Value *LLVMEmitter::EmitF64(F64 value) {
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(_llvmContext), value);
}

}//namespace elf

#endif // LLVMFLOAT

