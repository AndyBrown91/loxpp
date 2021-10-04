#include "Token.h"

Token::Token (Type t, const std::string_view lexeme, Object literal, int line)
: type (t)
, lexeme (lexeme)
, literal (literal)
, line (line)
{}

std::string Token::toString() const
{
    return typeToString (type) + " " + lexeme + " " + objectToString (literal);
}

std::string Token::typeToString (Type t)
{
    switch (t)
    {
        case Type::LEFT_PAREN: return "LEFT_PAREN";
        case Type::RIGHT_PAREN: return "RIGHT_PAREN";
        case Type::LEFT_BRACE: return "LEFT_BRACE";
        case Type::RIGHT_BRACE: return "RIGHT_BRACE";
        case Type::COMMA: return "COMMA";
        case Type::DOT: return "DOT";
        case Type::MINUS: return "MINUS";
        case Type::PLUS: return "PLUS";
        case Type::SEMICOLON: return "SEMICOLON";
        case Type::SLASH: return "SLASH";
        case Type::STAR: return "STAR";
        case Type::BANG: return "BANG";
        case Type::BANG_EQUAL: return "BANG_EQUAL";
        case Type::EQUAL: return "EQUAL";
        case Type::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case Type::GREATER: return "GREATER";
        case Type::GREATER_EQUAL: return "GREATER_EQUAL";
        case Type::LESS: return "LESS";
        case Type::LESS_EQUAL: return "LESS_EQUAL";
        case Type::IDENTIFIER: return "IDENTIFIER";
        case Type::STRING: return "STRING";
        case Type::NUMBER: return "NUMBER";
        case Type::AND: return "AND";
        case Type::CLASS: return "CLASS";
        case Type::ELSE: return "ELSE";
        case Type::FALSE: return "FALSE";
        case Type::FUN: return "FUN";
        case Type::FOR: return "FOR";
        case Type::IF: return "IF";
        case Type::NIL: return "NIL";
        case Type::OR: return "OR";
        case Type::PRINT: return "PRINT";
        case Type::RETURN: return "RETURN";
        case Type::SUPER: return "SUPER";
        case Type::THIS: return "THIS";
        case Type::TRUE: return "TRUE";
        case Type::VAR: return "VAR";
        case Type::WHILE: return "WHILE";
        case Type::EOF_: return "EOF";
        default: return "Unknown";
    }
}
