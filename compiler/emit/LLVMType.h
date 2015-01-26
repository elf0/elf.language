#ifndef LLVMTYPE
#define LLVMTYPE

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Object.h"

namespace elf{

llvm::Type *LLVMEmitter::Emit(ast::Object::Type type){
    llvm::Type *pLLVMType;
    switch (type) {
    default:
        pLLVMType = llvm::Type::getVoidTy(_llvmContext);
        break;
    case ast::Object::Type::otBoolean:
        pLLVMType = llvm::Type::getInt1Ty(_llvmContext);
        break;
    case ast::Object::Type::otInteger8:
        pLLVMType = llvm::Type::getInt8Ty(_llvmContext);
        break;
    case ast::Object::Type::otInteger16:
        pLLVMType = llvm::Type::getInt16Ty(_llvmContext);
        break;
    case ast::Object::Type::otInteger32:
        pLLVMType = llvm::Type::getInt32Ty(_llvmContext);
        break;
    case ast::Object::Type::otInteger64:
        pLLVMType = llvm::Type::getInt64Ty(_llvmContext);
        break;
    case ast::Object::Type::otFloat32:
        pLLVMType = llvm::Type::getFloatTy(_llvmContext);
        break;
    case ast::Object::Type::otFloat64:
        pLLVMType = llvm::Type::getDoubleTy(_llvmContext);
        break;
    }
    return pLLVMType;
}

}//namespace elf

#endif // LLVMTYPE

