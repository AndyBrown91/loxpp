#include "Lox.h"
#include "Scanner.h"

#include <iostream>

Lox::Lox(){}

void Lox::run (const std::string_view& script)
{
    foundError = false;

    Scanner s (*this, script);
    const auto tokens = s.scanTokens();

    for (auto& t : tokens)
        std::cout << t.toString() << '\n';
}

void Lox::reportError (int line, const std::string& where, const std::string& message)
{
    std::cerr << "[line " << line << "] Error " << where << ": " << message << '\n';
    foundError = true;
}
