

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>


using namespace llvm;

int main(){
    LLVMContext context;

    Module module("max_module",context);

    IRBuilder<> builder(context);

    Type *int32Type = builder.getInt32Ty();

    FunctionType *funcType =  FunctionType::get(int32Type,{int32Type,int32Type},false);

    Function *func = Function::Create(funcType,Function::ExternalLinkage,"max",module);


    auto arg = func->arg_begin();

    Value *a = &*arg++;
    a->setName("a");

    Value *b = &*arg++;
    b->setName("b");



    //创建基本块
    BasicBlock *entryBB = BasicBlock::Create(context,"entry",func);

    BasicBlock *thenBB = BasicBlock::Create(context,"then",func);

    BasicBlock *elseBB = BasicBlock::Create(context,"else",func);

    BasicBlock *mergeBB = BasicBlock::Create(context,"merge",func);


    //entry：比较 a > b
    builder.SetInsertPoint(entryBB);

    Value *cond = builder.CreateICmpSGT(a,b,"cond");


    //根据比较结果跳转
    builder.CreateCondBr(cond,thenBB,elseBB);

    //then: 选择a
    builder.SetInsertPoint(thenBB);
    builder.CreateBr(mergeBB);

    //else: 选择b
    builder.SetInsertPoint(elseBB);
    builder.CreateBr(mergeBB);

    // merge：合并两个分支的值
    builder.SetInsertPoint(mergeBB);


    PHINode *result = builder.CreatePHI(int32Type,2,"result");



    result->addIncoming(a,thenBB);
    result->addIncoming(b,elseBB);


    //返回结果
    builder.CreateRet(result);


    // 11. 验证 IR 是否正确
    if (verifyModule(module, &errs())) {
        errs() << "LLVM IR verification failed!\n";
        return 1;
    }

    // 12. 打印 LLVM IR
    module.print(outs(), nullptr);

    return 0;
}