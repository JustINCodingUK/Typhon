//
// Created by justinw on 12/03/2026.
//

#ifndef TYPHON_SYMBOL_H
#define TYPHON_SYMBOL_H
#include <memory>
#include <stack>
#include <unordered_map>

#include "Type.h"
#include "../parser/Common.h"

struct Symbol {
    std::string name;
    const Type type;
    Visibility visibility;
    bool isInitialized;

    Symbol(std::string name, Type type, const Visibility visibility, const bool isInitialized)
        : name(std::move(name)), type(std::move(type)), visibility(visibility), isInitialized(isInitialized) {}
};

class SymbolTable {
    std::stack<std::unordered_map<std::string, Symbol>> scopes;
public:
    SymbolTable();
    ~SymbolTable();

    void enterScope();
    void exitScope();
    void define(const std::string &name, Symbol sym);
    std::unique_ptr<Symbol> resolve(const std::string &name);
};


#endif //TYPHON_SYMBOL_H