//
// Created by justinw on 06/03/2026.
//

#ifndef PYC___ASTNODE_H
#define PYC___ASTNODE_H
#include <memory>
#include <utility>
#include <vector>

#include "Visibility.h"
#include "../lexer/Token.h"

struct ASTNode {
    virtual ~ASTNode();
};

struct Parameter : ASTNode {
    std::string name, type;

    Parameter(std::string name, std::string type) : name(std::move(name)), type(std::move(type)) {
    }
};

struct Function : ASTNode {
    std::string name, returnType;
    std::vector<Parameter> params;
    std::unique_ptr<ASTNode> body;
    Visibility visibility;

    Function(std::string name, std::string returnType, std::vector<Parameter> params, std::unique_ptr<ASTNode> body,
             const Visibility visibility)
        : name(std::move(name)), returnType(std::move(returnType)), params(std::move(params)), body(std::move(body)),
          visibility(visibility) {
    }
};

struct ExtensionFunction : ASTNode {
    std::string name, returnType, extensionOn;
    std::vector<Parameter> params;
    std::unique_ptr<ASTNode> body;
    Visibility visibility;

    ExtensionFunction(std::string name, std::string returnType, std::string extensionOn, std::vector<Parameter> params,
                      std::unique_ptr<ASTNode> body, const Visibility visibility)
        : name(std::move(name)), returnType(std::move(returnType)), extensionOn(std::move(extensionOn)),
          params(std::move(params)), body(std::move(body)), visibility(visibility) {
    }
};

struct Class : ASTNode {
    std::string name;
    std::vector<Parameter> params;
    std::unique_ptr<ASTNode> body;
    Visibility visibility;

    Class(std::string name, std::vector<Parameter> params, std::unique_ptr<ASTNode> body, const Visibility visibility)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)), visibility(visibility) {
    }
};

struct BinaryExpr : ASTNode {
    std::unique_ptr<ASTNode> left, right;
    Token op;

    BinaryExpr(Token op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : left(std::move(left)), right(std::move(right)), op(std::move(op)) {
    }
};

struct ConditionalStatement : ASTNode {
    std::unique_ptr<BinaryExpr> condition;
    std::unique_ptr<ASTNode> body;
    std::unique_ptr<ASTNode> elseNode;

    ConditionalStatement(std::unique_ptr<BinaryExpr> condition, std::unique_ptr<ASTNode> body,
                         std::unique_ptr<ASTNode> elseNode)
        : condition(std::move(condition)), body(std::move(body)), elseNode(std::move(elseNode)) {
    }
};

struct ForStatement : ASTNode {
    std::string identifier;
    std::unique_ptr<BinaryExpr> iterable;
    std::unique_ptr<ASTNode> body;

    ForStatement(std::string identifier, std::unique_ptr<BinaryExpr> iterable, std::unique_ptr<ASTNode> body)
        : identifier(std::move(identifier)), iterable(std::move(iterable)), body(std::move(body)) {};
};

struct WhileStatement : ASTNode {
    std::unique_ptr<BinaryExpr> condition;
    std::unique_ptr<ASTNode> body;

    WhileStatement(std::unique_ptr<BinaryExpr> condition, std::unique_ptr<ASTNode> body)
        : condition(std::move(condition)), body(std::move(body)) {
    };
};

struct Expression : ASTNode {};

struct Literal : Expression {
    std::string type;
    std::variant<std::string, double> value;

    Literal(std::string type, std::variant<std::string, double> value) : type(std::move(type)), value(std::move(value)) {};
};

struct FunctionCall : Expression {
    std::string caller;
    std::string functionName;
    std::vector<std::unique_ptr<Expression>> args;

    FunctionCall(std::string caller, std::string functionName, std::vector<std::unique_ptr<Expression>> args)
        : caller(std::move(caller)), functionName(std::move(functionName)), args(std::move(args)) {};
};

#endif //PYC___ASTNODE_H
