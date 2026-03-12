cmake_minimum_required(VERSION 4.1)
project(typhon)

set(CMAKE_CXX_STANDARD 23)

add_executable(typhon main.cpp
        lexer/Token.cpp
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/Token.h
        parser/Parser.cpp
        parser/Parser.h
        parser/ASTNode.cpp
        parser/ASTNode.h
        util/variant_utils.h
        util/variant_utils.cpp
        analyzer/SemanticAnalyzer.cpp
        analyzer/SemanticAnalyzer.h
        parser/Common.h
        analyzer/Symbol.cpp
        analyzer/Symbol.h
        analyzer/Type.cpp
        analyzer/Type.h
        analyzer/Visitor.h
        errors/errors.h)
