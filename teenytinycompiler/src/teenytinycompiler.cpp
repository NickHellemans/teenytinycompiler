#include <iostream>

#include "Lexer.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, my teeny tiny compiler!" << std::endl;
    const std::string input = "IF+-123\"This is a string\"  foo*THEN/";
    Lexer lexer(input);
    Token token = lexer.getToken();
    while (token.kind != TokenKind::EndOfFile && token.kind != TokenKind::BadToken)
    {
        std::cout << (int)token.kind << std::endl;
        std::cout << token.text << std::endl;
        token = lexer.getToken();
    }
    
    return 0;
}

