//
// Created by justinw on 06/03/2026.
//

#ifndef PYC___PARSER_H
#define PYC___PARSER_H
#include <functional>
#include <memory>
#include <vector>

#include "ASTNode.h"
#include "Common.h"

class Parser {
    int index = 0;
    std::vector<Token> tokens;
    Token peek();
    Token peek(int n);
    Token consume();
    Token consume(TokenType type, const std::string& error);
    Token consume(const std::function<bool(TokenType)>& predicate, const std::string& error);
    bool match(TokenType type);
    [[nodiscard]] bool isAtEnd() const;

public:
    explicit Parser(std::vector<Token> tokens);
    ~Parser();

    std::vector<std::unique_ptr<ASTNode>> parse();
    std::unique_ptr<ASTNode> classDeclaration(Visibility visibility);
    std::unique_ptr<ASTNode> functionDeclaration(Visibility visibility);
    std::unique_ptr<ASTNode> extensionFunctionDeclaration(Visibility visibility);
    std::vector<Parameter> parameterList();
    Parameter parameter();
    std::unique_ptr<ASTNode> conditionalStatement();
    std::unique_ptr<ASTNode> whileStatement();
    std::unique_ptr<ASTNode> forStatement();
    std::unique_ptr<Expression> expression();
    std::unique_ptr<ASTNode> block();

private:
    std::unique_ptr<Expression> parseExpression(int precedence);
    std::unique_ptr<Expression> parsePrefix();
    std::unique_ptr<Expression> parseInfix(std::unique_ptr<Expression> left, Token op);
    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);

    static int getPrecedence(TokenType type);
};


#endif //PYC___PARSER_H