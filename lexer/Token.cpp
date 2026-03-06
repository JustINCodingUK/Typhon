#include <string>
#include <variant>
#include "Token.h"

std::string Token::toString() {
    if (std::holds_alternative<std::string>(value)) {
        return std::get<std::string>(value);
    }
    return "";
}

Token Token::monostate(TokenType type, int line, int column) {
    return {type, std::monostate(), line, column};
}
