#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Verifier.h>


using namespace llvm;


/**
 * //实现下面函数
 int abs_value(int x) {
    if (x >= 0) {
        return x;
    } else {
        return -x;
    }
}
 */
int main(){

    LLVMContext context;

    Module module("abs",context);

    IRBuilder<> builder(context);

    Type *int32Ty = builder.getInt32Ty();


    FunctionType *funcType = FunctionType::get(int32Ty,{int32Ty},false);


    Function *func = Function::Create(funcType,Function::ExternalLinkage,"abs",module);


    auto arg = func->arg_begin();

    Value *x = &*arg++;
    x->setName("x");


    BasicBlock *entryBB = BasicBlock::Create(context,"entry",func);
    BasicBlock *thenBB = BasicBlock::Create(context,"then",func);
    BasicBlock *elseBB = BasicBlock::Create(context,"else",func);
    BasicBlock *mergeBB = BasicBlock::Create(context,"merge",func);



    builder.SetInsertPoint(entryBB);


    Value *zero = builder.getInt32(0);
    Value *cond = builder.CreateICmpSGE(x,zero,"cond");

    //创建 a > 0 的指令
    builder.CreateCondBr(cond,thenBB,elseBB);

    //x>0
    builder.SetInsertPoint(thenBB);
    builder.CreateBr(mergeBB);

    //x<0
    builder.SetInsertPoint(elseBB);
    Value *negative = builder.CreateNeg(x,"negative");
    builder.CreateBr(mergeBB);

    //merge: PHI 合并结果
    builder.SetInsertPoint(mergeBB);
    PHINode *result = builder.CreatePHI(int32Ty,2,"result");
    result->addIncoming(x,thenBB);
    result->addIncoming(negative,elseBB);

    //返回结果
    builder.CreateRet(result);


       // 11. 验证 LLVM IR
    if (verifyModule(module, &errs())) {
        errs() << "IR verification failed!\n";
        return 1;
    }

    // 12. 输出 LLVM IR
    module.print(outs(), nullptr);

    return 0;

}





