#include "Parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
}

Parser::~Parser() = default;

Token Parser::peek() {
    return tokens[index];
}

Token Parser::peek(const int n) {
    if (index + n < tokens.size()) return tokens[index + n];
    return tokens[tokens.size() - 1];
}

Token Parser::consume() {
    if (!isAtEnd()) index++;
    return tokens[index - 1];
}

Token Parser::consume(const TokenType type, const std::string &error) {
    if (match(type)) return consume();
    perror(
        (error + " at line " + std::to_string(tokens[index].line) + " column " + std::to_string(tokens[index].column) +
         "\n").c_str());
    exit(100);
}

Token Parser::consume(const std::function<bool(TokenType)> &predicate, const std::string &error) {
    if (predicate(peek().type)) return consume();
    perror(
        (error + " at line " + std::to_string(tokens[index].line) + " column " + std::to_string(tokens[index].column) +
         "\n").c_str());
    exit(100);
}

bool Parser::match(const TokenType type) {
    if (tokens[index].type == type) {
        consume();
        return true;
    }
    return false;
}

bool Parser::isAtEnd() const {
    return tokens[index].type == TokenType::EndOfFile;
}

std::vector<std::unique_ptr<ASTNode> > Parser::parse() {
}

std::unique_ptr<ASTNode> Parser::classDeclaration(Visibility visibility) {
    consume(TokenType::Class, "Expected class definition");
    Token name = consume(TokenType::Identifier, "Expected class name");
    auto parameters = parameterList();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    return std::make_unique<Class>(name.toString(), parameters, std::move(body), visibility);
}

std::unique_ptr<ASTNode> Parser::functionDeclaration(Visibility visibility) {
    consume(TokenType::Def, "Expected function definition");
    Token name = consume(TokenType::Identifier, "Expected function name");
    auto parameters = parameterList();
    consume(TokenType::Arrow, "Expected return type");
    Token returnType = consume(TokenType::Identifier, "Expected return type");
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<Function>(name.toString(), returnType.toString(), parameters, std::move(body), visibility);
}

std::unique_ptr<ASTNode> Parser::extensionFunctionDeclaration(Visibility visibility) {
    consume(TokenType::Ext, "Expected extension function definition");
    consume(TokenType::Def, "Expected extension function definition");
    auto extensionOn = consume(TokenType::Identifier, "Expected extension type");
    consume(TokenType::Dot, "Expected '.'");
    Token name = consume(TokenType::Identifier, "Expected extension function name");
    auto parameters = parameterList();
    consume(TokenType::Arrow, "Expected return type");
    Token returnType = consume(TokenType::Identifier, "Expected return type");
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<ExtensionFunction>(name.toString(), returnType.toString(), extensionOn.toString(),
                                               parameters,
                                               std::move(body), visibility);
}

std::vector<Parameter> Parser::parameterList() {
    consume(TokenType::LParen, "Expected '('");
    std::vector<Parameter> params;
    if (!match(TokenType::RParen)) {
        params.push_back(parameter());
        while (match(TokenType::Comma)) params.push_back(parameter());
    }
    return params;
}

Parameter Parser::parameter() {
    Token name = consume(TokenType::Identifier, "Expected parameter name");
    consume(TokenType::Colon, "Expected parameter declaration");
    Token type = consume(TokenType::Identifier, "Expected parameter type");
    return {name.toString(), type.toString()};
}

std::unique_ptr<ASTNode> Parser::conditionalStatement() {
    consume(TokenType::If, "Expected if");
    auto condition = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    std::unique_ptr<ASTNode> elseNode;
    if (match(TokenType::Else)) {
        consume(TokenType::Else, "Expected else");
        if (match(TokenType::If)) {
            elseNode = conditionalStatement();
        } else {
            consume(TokenType::LBrace, "Expected body");
            elseNode = block();
        }
    }
    return std::make_unique<ConditionalStatement>(std::move(condition), std::move(body), std::move(elseNode));
}

std::unique_ptr<ASTNode> Parser::whileStatement() {
    consume(TokenType::While, "Expected while loop declaration");
    auto condition = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::forStatement() {
    consume(TokenType::For, "Expected for loop declaration");
    auto identifier = consume(TokenType::Identifier, "Expected identifier");
    consume(TokenType::In, "Expected in");
    auto iterable = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    return std::make_unique<ForStatement>(identifier.toString(), std::move(iterable), std::move(body));
}

std::unique_ptr<ASTNode> Parser::block() {
    auto visibilityModifier = Visibility::PUBLIC;
    switch (peek().type) {
        case TokenType::Public:
            consume(TokenType::Public, "Expected visibility modifier");
            break;
        case TokenType::Private:
            consume(TokenType::Private, "Expected visibility modifier");
            visibilityModifier = Visibility::PRIVATE;
            break;
        case TokenType::Internal:
            consume(TokenType::Internal, "Expected visibility modifier");
            visibilityModifier = Visibility::INTERNAL;
            break;
        default: ;
    }

    switch (peek().type) {
        case TokenType::If:
            return conditionalStatement();
        case TokenType::For:
            return forStatement();
        case TokenType::While:
            return whileStatement();
        case TokenType::Def:
            return functionDeclaration(visibilityModifier);
        case TokenType::Ext:
            return extensionFunctionDeclaration(visibilityModifier);
        case TokenType::Class:
            return classDeclaration(visibilityModifier);
        case TokenType::Newline:
            consume();
            return block();
        default:
            return expression();
    }
}

std::unique_ptr<Expression> Parser::expression() {
    return parseExpression(NONE);
}

std::unique_ptr<Expression> Parser::parseExpression(const int precedence) {
    auto left = parsePrefix();
    while (true) {
        if (const int nextPrecedence = getPrecedence(peek().type); nextPrecedence <= precedence || isAtEnd()) break;
        const auto op = consume([](const TokenType type) {
            return type >= TokenType::Dot && type <= TokenType::UnaryPlus;
        }, "Expected operator");
        left = parseInfix(std::move(left), op);
    }
    return left;
}

std::unique_ptr<Expression> Parser::parsePrefix() {
    switch (Token next = consume(); next.type) {
        case TokenType::IntLiteral:
        case TokenType::StringLiteral:
        case TokenType::FloatLiteral:
        case TokenType::True:
        case TokenType::False:
            return std::make_unique<Literal>(next);
        case TokenType::Identifier:
            return std::make_unique<Identifier>(next.toString());
        case TokenType::LParen: {
            auto expr = parseExpression(Precedence::NONE);
            consume(TokenType::RParen, "Expected ')'");
            return expr;
        }
        case TokenType::UnaryMinus:
        case TokenType::UnaryPlus:
        case TokenType::Increment:
        case TokenType::Decrement:
        case TokenType::Bang:
            return std::make_unique<UnaryExpression>(next, std::move(parsePrefix()));
        default:
            throw std::runtime_error("Expected expression");
    }
}

std::unique_ptr<Expression> Parser::parseInfix(std::unique_ptr<Expression> left, Token op) {
    int precedence = getPrecedence(op.type);
    switch (op.type) {
        case TokenType::Dot: {
            auto functionName = consume(TokenType::Identifier, "Expected property name after '.'");
            return std::make_unique<GetExpression>(std::move(left), functionName.toString());
        }

        case TokenType::LParen:
            return finishCall(std::move(left));

        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Multiply:
        case TokenType::Divide:
        case TokenType::Modulo:
        case TokenType::FloorDivide:
        case TokenType::Power: {
            auto right = parseExpression(precedence);
            return std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
        }

        case TokenType::PlusAssign:
        case TokenType::MinusAssign:
        case TokenType::MultiplyAssign:
        case TokenType::DivideAssign:
        case TokenType::FloorDivideAssign:
        case TokenType::ModuloAssign:
        case TokenType::PowerAssign: {
            auto value = parseExpression(ASSIGNMENT - 1);
            if (auto* var = dynamic_cast<VariableExpression*>(left.get())) {
                return std::make_unique<CompoundAssignExpression>(var->name, op, std::move(value));
            }
            if (auto* get = dynamic_cast<GetExpression*>(left.get())) {
                return std::make_unique<CompoundSetExpression>(std::move(get->left), get->propertyName, op, std::move(value));
            }
            throw std::runtime_error("Invalid assignment target");
        }
        case TokenType::Assign: {
            auto value = parseExpression(ASSIGNMENT - 1);
            return std::make_unique<AssignExpression>(std::move(left), std::move(value));
        }
        default:
            return left;
    }
}

std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
    std::vector<std::unique_ptr<Expression>> args;
    if (!match(TokenType::RParen)) {
        do args.push_back(parseExpression(NONE));
        while (match(TokenType::Comma));

        consume(TokenType::RParen, "Expected ')' after arguments");
    }

    return std::make_unique<FunctionCallExpression>(std::move(callee), std::move(args));
}

int Parser::getPrecedence(const TokenType type) {
    switch (type) {
        case TokenType::PlusAssign:
        case TokenType::MinusAssign:
        case TokenType::MultiplyAssign:
        case TokenType::DivideAssign:
        case TokenType::FloorDivideAssign:
        case TokenType::ModuloAssign:
        case TokenType::PowerAssign:
        case TokenType::Assign: return ASSIGNMENT;
        case TokenType::Or: return OR;
        case TokenType::And: return AND;
        case TokenType::Equal:
        case TokenType::NotEqual: return EQUALITY;
        case TokenType::LessThan:
        case TokenType::LessThanOrEqual:
        case TokenType::GreaterThan:
        case TokenType::GreaterThanOrEqual: return COMPARISON;
        case TokenType::Plus:
        case TokenType::Minus: return TERM;
        case TokenType::Multiply:
        case TokenType::Divide:
        case TokenType::FloorDivide:
        case TokenType::Modulo: return FACTOR;
        case TokenType::Power: return EXPONENT;
        case TokenType::LParen:
        case TokenType::Dot: return CALL;
        default: return NONE;
    }
}