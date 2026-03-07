#ifndef PYC___TOKEN_H
#define PYC___TOKEN_H
#include <string>
#include <variant>

#endif //PYC___TOKEN_H

enum class TokenType {
    // Symbols
    LParen, RParen, LBracket, RBracket, LBrace, RBrace, Comma, Dot, Semicolon, Colon, Arrow, Newline, EndOfFile,
    // Binary Operator
    Plus, Minus, Multiply, Divide, Modulo, Power, FloorDivide, Assign, Equal, NotEqual, LessThan,
    GreaterThan, PlusAssign, MinusAssign, MultiplyAssign, DivideAssign, ModuloAssign, PowerAssign,
    FloorDivideAssign, LessThanOrEqual, GreaterThanOrEqual, DoubleAmpersand, DoublePipe,
    // Unary Operator
    Increment, Decrement, UnaryMinus, UnaryPlus,
    // Keywords
    If, Else, Elif, While, For, Def, Class, Return, Ext, Import, As, Try, Catch, Finally, With,
    Break, Continue, And, Or, Not, In, Is, True, False, Nonlocal, Del, Assert, Async, Await, Public, Internal, Private,
    // Constants
    Identifier, Literal

};

struct Token {
    TokenType type;
    std::variant<std::monostate, std::string, double> value;
    int line, column;

    std::string toString();

    static Token monostate(TokenType type, int line, int column);
};