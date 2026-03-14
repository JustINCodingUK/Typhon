//
// Created by justinw on 10/03/2026.
//

#ifndef TYPHON_SEMANTIC_ANALYZER_H
#define TYPHON_SEMANTIC_ANALYZER_H
#include "SymbolTable.h"
#include "common/Visitor.h"

class SemanticAnalyzer : public Visitor {
    SymbolTable symbolTable;
public:
    SemanticAnalyzer();
    ~SemanticAnalyzer() override;

    void visit(BinaryExpr *expr) override;
    void visit(ConditionalStatement *expr) override;
    void visit(ForStatement *expr) override;
    void visit(WhileStatement *expr) override;
    void visit(Identifier *expr) override;
    void visit(UnaryExpression *expr) override;
    void visit(Literal *expr) override;
    void visit(FunctionCallExpression *expr) override;
    void visit(GetExpression *expr) override;
    void visit(VariableExpression *expr) override;
    void visit(CompoundAssignExpression *expr) override;
    void visit(CompoundSetExpression *expr) override;
    void visit(AssignExpression *expr) override;
    void visit(Class *expr) override;
    void visit(Function *expr) override;
    void visit(ExtensionFunction *expr) override;
    void visit(Parameter *expr) override;

private:
    void defineParameters(SymbolTable& thisSymbolTable, std::vector<Parameter>& params);
};


#endif //TYPHON_SEMANTIC_ANALYZER_H