#include <fstream>
#include <iostream>
#include "Lexer.h"
#include "Parser.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, my teeny tiny compiler!" << std::endl;
    //const std::string input = "IF+-123\"This is a string\"  foo*THEN/";
    std::ifstream sourceCode(R"(C:\dev\teenytinycompiler\teenytinycompiler\src\input.txt)");
    std::string input((std::istreambuf_iterator<char>(sourceCode)),
                 std::istreambuf_iterator<char>());

    if(sourceCode.is_open())
    {
        std::string line;
        while(std::getline(sourceCode, line))
        {
            std::cout << line << std::endl;
        }
        sourceCode.close();
    }

    Lexer lexer(input);
    Parser parser(lexer);

    parser.program();
    std::cout << "Parsing complete!\n";
    //Token token = lexer.getToken();
    // while (token.kind != TokenKind::EndOfFile && token.kind != TokenKind::BadToken)
    // {
    //     std::cout << (int)token.kind << std::endl;
    //     std::cout << token.text << std::endl;
    //     token = lexer.getToken();
    // }
    
    return 0;
}

