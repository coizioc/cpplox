#include <cstdio>
#include <memory>

#include "codegen.hpp"
#include "node.hpp"

extern Program* program;
extern FILE* yyin;
extern int yyparse();

void set_yyin(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr) {
            perror("Unable to open file");
            exit(1);
        }
    } else {
        yyin = stdin;
    }
}

int main(int argc, char** argv) {
#ifdef DEBUG
    std::cout << "Start parsing." << std::endl;
#endif
    set_yyin(argc, argv);
    yyparse();
#ifdef DEBUG
    std::cout << "Finish parsing." << std::endl;
#endif

    auto context = CodeGenContext();
    context.generateCode(*program);

    return 0;
}