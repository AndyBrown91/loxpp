#pragma once

#include <string>

class Lox
{
public:

    Lox();

    void run (const std::string_view& script);

    bool hadError() const { return foundError; }

    void reportError (int line, const std::string& where, const std::string& message);

private:

    bool foundError = false;
};
