#include <iostream>
#include <fstream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main(int argc, char** argv) {
    if (argc<2) {
        std::cerr << "No input files";
        return 1;
    }

    Lexer lexer;
    for (int i=1;i<argc;++i) {
        if (std::ifstream file(argv[i]); file.is_open()) {
            std::string contents((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
            auto tokens = lexer.tokenize(contents);
        }
    }
}
