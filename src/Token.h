#pragma once

#include <variant>
#include <string>

struct NullType{};
using Object = std::variant<NullType, bool, double, std::string>;

template<class> inline constexpr bool always_false_v = false;

inline std::string objectToString (const Object& o)
{
    std::string output;

    std::visit ([&output](auto&& arg)
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, NullType>)
            output = "Null";
        else if constexpr (std::is_same_v<T, bool>)
            output = std::to_string (arg);
        else if constexpr (std::is_same_v<T, double>)
            output = std::to_string (arg);
        else if constexpr (std::is_same_v<T, std::string>)
            output = arg;
        else
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
    }, o);

    return output;
}

class Token
{
public:

    enum class Type
    {
        // Single Char Tokens
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

        // One or Two character tokens
        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL,

        //Literals
        IDENTIFIER, STRING, NUMBER,

        // Keywords
        AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT,
        RETURN, SUPER, THIS, TRUE, VAR, WHILE, EOF_
    };

    Token (Type t, const std::string_view lexeme, Object literal, int line);

    std::string toString() const;

    static std::string typeToString (Type t);
private:
    Type type;
    std::string lexeme;
    Object literal;
    int line = -1;

};
