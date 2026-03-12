//
// Created by justinw on 06/03/2026.
//

#ifndef TYPHON_ASTNODE_H
#define TYPHON_ASTNODE_H
#include <memory>
#include <utility>
#include <vector>

#include "Common.h"
#include "../analyzer/Type.h"
#include "../lexer/Token.h"
#include "../analyzer/Visitor.h"

struct ASTNode {
    virtual ~ASTNode();
    virtual void accept(Visitor* visitor) = 0;
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

    void accept(Visitor* visitor) override {
        visitor->visit(this);
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

    void accept(Visitor* visitor) override {
        visitor->visit(this);
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

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};


struct Expression : ASTNode {
    ~Expression() override = default;
    Type evaluatedType;
};

struct BinaryExpr : Expression {
    std::unique_ptr<Expression> left, right;
    Token op;

    BinaryExpr(Token op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left(std::move(left)), right(std::move(right)), op(std::move(op)) {
    }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct ConditionalStatement : ASTNode {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<ASTNode> body;
    std::unique_ptr<ASTNode> elseNode;

    ConditionalStatement(std::unique_ptr<Expression> condition, std::unique_ptr<ASTNode> body,
                         std::unique_ptr<ASTNode> elseNode)
        : condition(std::move(condition)), body(std::move(body)), elseNode(std::move(elseNode)) {
    }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct WhileStatement : ASTNode {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<ASTNode> body;

    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<ASTNode> body)
        : condition(std::move(condition)), body(std::move(body)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct Identifier : Expression {
    std::string name;
    Type evaluatedType;

    Identifier(std::string name) : name(std::move(name)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct ForStatement : ASTNode {
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<Expression> iterable;
    std::unique_ptr<ASTNode> body;

    ForStatement(std::unique_ptr<Identifier> identifier, std::unique_ptr<Expression> iterable, std::unique_ptr<ASTNode> body)
        : identifier(std::move(identifier)), iterable(std::move(iterable)), body(std::move(body)) {
    }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    };
};

struct UnaryExpression : Expression {
    Token op;
    std::unique_ptr<Expression> right;

    UnaryExpression(Token op, std::unique_ptr<Expression> right) : op(std::move(op)), right(std::move(right)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct Literal : Expression {
    Token token;
    Type evaluatedType;

    Literal(Token token) : token(std::move(token)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct FunctionCallExpression : Expression {
    std::unique_ptr<Expression> callee;
    std::vector<std::unique_ptr<Expression>> args;
    Type evaluatedType;

    FunctionCallExpression(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression> > args)
        : callee(std::move(callee)), args(std::move(args)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct GetExpression : Expression {
    std::unique_ptr<Expression> left;
    std::string propertyName;
    Type evaluatedType;

    GetExpression(std::unique_ptr<Expression> left, std::string propertyName) : left(std::move(left)),
        propertyName(std::move(propertyName)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct VariableExpression : Expression {
    std::string name;
    Type evaluatedType;

    VariableExpression(std::string name) : name(std::move(name)) {
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct CompoundAssignExpression : Expression {
    std::string name;
    Token op;
    std::unique_ptr<Expression> value;
    Type evaluatedType;

    CompoundAssignExpression(std::string name, Token op, std::unique_ptr<Expression> value) : name(std::move(name)),
        op(std::move(op)), value(std::move(value)) {
    }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct CompoundSetExpression : Expression {
    std::unique_ptr<Expression> object;
    std::string propertyName;
    Token op;
    std::unique_ptr<Expression> value;
    Type evaluatedType;

    CompoundSetExpression(std::unique_ptr<Expression> object, std::string propertyName, Token op, std::unique_ptr<Expression> value)
        : object(std::move(object)), propertyName(std::move(propertyName)), op(std::move(op)), value(std::move(value)) {
    }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

struct AssignExpression : Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> value;
    Type evaluatedType;

    AssignExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> value)
        : left(std::move(left)), value(std::move(value)) {}

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

#endif //TYPHON_ASTNODE_H
