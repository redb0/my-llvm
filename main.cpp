#include "tokenizer.h"
#include "parser.h"

//static std::unique_ptr<llvm::Module> TheModule;
//static llvm::LLVMContext TheContext;

//int main() {
//    // получение массива с токенами.
//    fprintf(stderr, "ready> ");

//    std::string src;
//    std::string file_name = "code.txt";
//    src = open_file(file_name);

//    std::vector<token> vec{};
//    vec = get_tokens(src);

//    // модуль который содержит весь код.
//    TheModule = llvm::make_unique<llvm::Module>("my cool jit", TheContext);

//    // Run the main "interpreter loop" now.
//    MainLoop();

//    // Вывод сгенерированного кода.
//    TheModule->print(errs(), nullptr);

//    return 0;
//}
