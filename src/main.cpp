#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "Lox.h"

namespace fs = std::filesystem;

std::string readFile(const fs::path& path)
{
    if (! fs::exists (path))
    {
        std::cerr << "Path doesn't exist: " << path << '\n';
        return "";
    }

    // Open the stream to 'lock' the file.
    std::ifstream f (path, std::ios::in | std::ios::binary);

    // Obtain the size of the file.
    const auto sz = fs::file_size(path);

    // Create a buffer.
    std::string result(sz, '\0');

    // Read the whole file into the buffer.
    f.read(result.data(), sz);

    return result;
}

bool runFile (const fs::path& path)
{
    Lox l;
    l.run (readFile(path));

    return l.hadError();
}

void runPrompt()
{
    Lox l;
    std::string input;

    while (true)
    {
        std::cout << "> ";
        std::cin >> input;

        if (input == "exit")
            break;

        l.run (input);
    }
}

int main (int argc, char** argv)
{
    if (argc > 2)
    {
        std::cerr << "Usage: lox [script]\n";
        return 1;
    }

    if (argc == 2)
        return runFile (argv[1]);

    runPrompt();

    return 0;
}
