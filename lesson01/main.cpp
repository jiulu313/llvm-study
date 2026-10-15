
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Support/raw_ostream.h>

#include <memory>

using namespace llvm;

int main() {
    // 1. 创建 LLVM 上下文
    LLVMContext context;

    // 2. 创建 LLVM 模块
    Module module("my_module", context);

    // 3. 创建 IR 指令构建器
    IRBuilder<> builder(context);

    // 4. 创建函数类型：int add(int, int)
    Type *int32Type = builder.getInt32Ty();

    FunctionType *funcType = FunctionType::get(int32Type,  {int32Type, int32Type},false);

    // 5. 创建 add 函数
    // Function *func = Function::Create(
    //     funcType,
    //     Function::ExternalLinkage,
    //     "add",
    //     module
    // );


    Function *func = Function::Create(funcType,Function::ExternalLinkage,"calculate",module);




    // 6. 设置参数名称
    auto arg = func->arg_begin();
    
    

    Value *a = &*arg++;
    a->setName("a");

    Value *b = &*arg++;
    b->setName("b");

    // 7. 创建基本块 entry
    BasicBlock *entry = BasicBlock::Create(
        context,
        "entry",
        func
    );

    builder.SetInsertPoint(entry);

    // 8. 生成加法指令
    Value *sum = builder.CreateAdd(
        a,
        b,
        "sum"
    );

    Value *two = builder.getInt32(2);
    Value *result = builder.CreateMul(
        sum,
        two 
    );

    // 9. 生成返回指令
    builder.CreateRet(result);

    // 10. 输出 LLVM IR
    module.print(outs(), nullptr);

    return 0;
}
