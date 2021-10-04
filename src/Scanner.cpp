#include "Scanner.h"

#include <unordered_map>

using TT = Token::Type;

std::string_view substr (const std::string_view& str, int start, int end)
{
    return str.substr (start, end - start);
}

bool isAlpha (char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool isAlphaNumeric (char c)
{
    return isdigit (c) || isAlpha (c);
}

const std::unordered_map<std::string, TT>& getKeywords()
{
    static std::unordered_map<std::string, TT> keywords =
    {
        { "and",   TT::AND},
        { "class", TT::CLASS},
        { "else",  TT::ELSE},
        { "false", TT::FALSE},
        { "for",   TT::FOR},
        { "fun",   TT::FUN},
        { "if",    TT::IF},
        { "nil",   TT::NIL},
        { "or",    TT::OR},
        { "print", TT::PRINT},
        { "return",TT::RETURN},
        { "super", TT::SUPER},
        { "this",  TT::THIS},
        { "true",  TT::TRUE},
        { "var",   TT::VAR},
        { "while", TT::WHILE}
    };

    return keywords;
}

Scanner::Scanner (Lox& p, const std::string_view script)
: parent (p)
, source (script)
{

}

std::vector<Token> Scanner::scanTokens()
{
    while (! isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.emplace_back (TT::EOF_, "", NullType(), line);
    return tokens;
}

void Scanner::scanToken()
{


    char c = advance();
    switch (c)
    {
        case '(': addToken (TT::LEFT_PAREN); break;
        case ')': addToken (TT::RIGHT_PAREN); break;
        case '{': addToken (TT::LEFT_BRACE); break;
        case '}': addToken (TT::RIGHT_BRACE); break;
        case ',': addToken (TT::COMMA); break;
        case '.': addToken (TT::DOT); break;
        case '-': addToken (TT::MINUS); break;
        case '+': addToken (TT::PLUS); break;
        case ';': addToken (TT::SEMICOLON); break;
        case '*': addToken (TT::STAR); break;

        case '!':
            addToken(match('=') ? TT::BANG_EQUAL : TT::BANG);
            break;
        case '=':
            addToken(match('=') ? TT::EQUAL_EQUAL : TT::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TT::LESS_EQUAL : TT::LESS);
            break;
        case '>':
            addToken(match('=') ? TT::GREATER_EQUAL : TT::GREATER);
            break;

        case '/':
            if (match ('/'))
            {
                // Comment
                while (peek() != '\n' && ! isAtEnd())
                    advance();
            }
            else
            {
                addToken (TT::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;

        case '"':
            parseString();
            break;

        default:
            if (isdigit (c))
                parseNumber();
            else if (isAlpha (c))
                parseIdentifier();
            else
                parent.reportError (line, "", "Unexpected character.");
    }
}

char Scanner::peek()
{
    if (isAtEnd()) return '\n';
    return source[current];
}

char Scanner::peekNext()
{
    if (current + 1 >= source.size()) return '\n';
    return source[current + 1];
}

void Scanner::parseString()
{
    while (peek() != '"' && ! isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd())
    {
        parent.reportError (line, "", "Unterminated String");
        return;
    }

    advance();
    std::string_view str = substr (source, start + 1, current - 1); // Remove quote chars
    addToken (TT::STRING, std::string (str));
}

void Scanner::parseNumber()
{
    while (isdigit (peek()))
        advance();

    if (peek() == '.' && isdigit (peekNext()))
    {
        advance();

        while (isdigit (peek()))
            advance();
    }

    addToken (TT::NUMBER, atof (substr (source, start, current).data()));
}

void Scanner::parseIdentifier()
{
    while (isAlphaNumeric (peek()))
        advance();

    std::string_view text = substr (source, start, current);

    if (auto iter = getKeywords().find (std::string (text)); iter != getKeywords().end())
        addToken (iter->second);
    else
        addToken (TT::IDENTIFIER);
}

bool Scanner::match (char expected)
{
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Scanner::advance()
{
    return source[current++];
}

void Scanner::addToken (TT t)
{
    addToken (t, NullType());
}

void Scanner::addToken (TT t, Object literal)
{
    tokens.emplace_back (t, substr (source, start, current), literal, line);
}

bool Scanner::isAtEnd() const
{
    return current >= source.size();
}

