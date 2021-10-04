#pragma once

#include "Token.h"
#include "Lox.h"

#include <vector>

class Scanner
{
public:

    Scanner (Lox& p, const std::string_view script);

    std::vector<Token> scanTokens();

private:
    void scanToken();

    char advance();

    bool match (char expected);
    char peek();
    char peekNext();

    void parseString();
    void parseNumber();
    void parseIdentifier();

    void addToken (Token::Type t);
    void addToken (Token::Type t, Object literal);

    bool isAtEnd() const;

    Lox& parent;

    int start = 0;
    int line = 0;
    int current = 0;

    const std::string_view source;
    std::vector<Token> tokens;
};
