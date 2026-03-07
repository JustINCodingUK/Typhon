#include "Lexer.h"

#include <unordered_map>

Lexer::Lexer() = default;
Lexer::~Lexer() = default;

inline static const std::unordered_map<std::string_view, TokenType> tokenMap = {
        {"(", TokenType::LParen},
        {")", TokenType::RParen},
        {"[", TokenType::LBracket},
        {"]", TokenType::RBracket},
        {"{", TokenType::LBrace},
        {"}", TokenType::RBrace},
        {",", TokenType::Comma},
        {".", TokenType::Dot},
        {";", TokenType::Semicolon},
        {"->", TokenType::Arrow},
        {"+", TokenType::Plus},
        {"-", TokenType::Minus},
        {"*", TokenType::Multiply},
        {"/", TokenType::Divide},
        {"%", TokenType::Modulo},
        {"**", TokenType::Power},
        {"//", TokenType::FloorDivide},
        {"=", TokenType::Assign},
        {"==", TokenType::Equal},
        {"!=", TokenType::NotEqual},
        {"<", TokenType::LessThan},
        {">", TokenType::GreaterThan},
        {"+=", TokenType::PlusAssign},
        {"-=", TokenType::MinusAssign},
        {"*=", TokenType::MultiplyAssign},
        {"/=", TokenType::DivideAssign},
        {"%=", TokenType::ModuloAssign},
        {"**=", TokenType::PowerAssign},
        {"//=", TokenType::FloorDivideAssign},
        {"<=", TokenType::LessThanOrEqual},
        {">=", TokenType::GreaterThanOrEqual},
        {"&&", TokenType::DoubleAmpersand},
        {"||", TokenType::DoublePipe},
        {"++", TokenType::Increment},
        {"--", TokenType::Decrement},
        {"-", TokenType::UnaryMinus},
        {"+", TokenType::UnaryPlus},
        {"\n", TokenType::Newline},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"elif", TokenType::Elif},
        {"while", TokenType::While},
        {"for", TokenType::For},
        {"def", TokenType::Def},
        {"class", TokenType::Class},
        {"return", TokenType::Return},
        {"ext", TokenType::Ext},
        {"import", TokenType::Import},
        {"as", TokenType::As},
        {"try", TokenType::Try},
        {"catch", TokenType::Catch},
        {"finally", TokenType::Finally},
        {"with", TokenType::With},
        {"break", TokenType::Break},
        {"continue", TokenType::Continue},
        {"and", TokenType::And},
        {"or", TokenType::Or},
        {"not", TokenType::Not},
        {"in", TokenType::In},
        {"is", TokenType::Is},
        {"True", TokenType::True},
        {"False", TokenType::False},
        {"nonlocal", TokenType::Nonlocal},
        {"del", TokenType::Del},
        {"assert", TokenType::Assert},
        {"async", TokenType::Async},
        {"await", TokenType::Await},
        {"public", TokenType::Public},
        {"private", TokenType::Private},
        {"internal", TokenType::Internal}
};

void flush(std::string &builder, std::vector<std::string> &rawTokens) {
    if (!builder.empty()) {
        rawTokens.push_back(builder);
        builder.clear();
    }
}

std::vector<std::string> Lexer::tokenize_raw(std::string &file) {
    std::vector<std::string> rawTokens;
    std::string builder;

    bool isStr = false;
    for (int i=0;i<file.size();++i) {
        char c = file[i];
        if (isStr) {
            builder+=c;
            if (c=='\"' && file[i-1]!='\\') {
                flush(builder, rawTokens);
                isStr = false;
            }
        } else if (c=='\"') {
            flush(builder, rawTokens);
            builder+=c;
            isStr=true;
        } else if (c=='\n') rawTokens.emplace_back(1, c);
        else if (std::isspace(c)) flush(builder, rawTokens);
        else if (std::isalnum(c)) {
            if (!builder.empty() && !std::isalnum(builder[0])) flush(builder, rawTokens);
            builder+=c;
        } else if (c=='(' || c==')') {
            flush(builder, rawTokens);
            rawTokens.emplace_back(1,c);
        } else if (c=='.' && std::isdigit(file[i-1])) builder+=c;
        else {
            if (!builder.empty() && std::isalnum(builder[0])) flush(builder, rawTokens);
            builder+=c;
        }
    }

    flush(builder, rawTokens);
    return rawTokens;
}

std::vector<Token> Lexer::tokenize(const std::vector<std::string> &rawTokens) {
    std::vector<Token> tokens;
    int line = 1, column = 0;
    for (auto &s: rawTokens) {
        if (tokenMap.contains(s)) {
            auto token = Token::monostate(tokenMap.at(s), line, column);
            tokens.push_back(token);
        } else if (s[0]=='"' && s.back()=='"') {
            auto token = Token(TokenType::Literal, s, line, column);
            tokens.push_back(token);
        } else if (std::isdigit(s[0])) {
            auto token = Token(TokenType::Literal, std::stod(s), line, column);
            tokens.push_back(token);
        } else {
            auto token = Token(TokenType::Identifier, s, line, column);
            tokens.push_back(token);
        }
        column+=s.size();
        if (s == "\n") {
            line++; column=0;
        }
    }
    tokens.emplace_back(TokenType::EndOfFile, std::monostate(), line, column);
    return tokens;
}

std::vector<Token> Lexer::tokenize(std::string &file) {
    std::vector<std::string> raw_tokens = tokenize_raw(file);
    return this->tokenize(raw_tokens);
}