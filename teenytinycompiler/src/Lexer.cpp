#include "Lexer.h"
#include <iostream>
#include <vector>

Lexer::Lexer(const std::string& input)
    :source(input + '\n') //Append a newline to simplify lexing/parsing the last token/statement
{
    nextChar();
}

void Lexer::nextChar()
{
    currPos += 1;
    if (currPos >= (int) source.length())
        currChar = '\0'; //End of file marker
    else
        currChar = source[currPos];
}

//Look at next char without updating
char Lexer::peek() const
{
    if(currPos + 1 >= (int) source.length())
        return '\0';
    return source[currPos + 1];
}

void Lexer::abort(const std::string& message)
{
    std::cout << message << std::endl;
}

void Lexer::skipWhitespace()
{
    while(currChar == ' ' || currChar == '\t' || currChar == '\r')
        nextChar();
}

void Lexer::skipComment()
{
    if (currChar == '#')
    {
        while (currChar != '\n')
            nextChar();
    }
}

Token Lexer::getToken()
{
    skipWhitespace();
    skipComment();
    
    Token token{"Invalid", TokenKind::BadToken};
    int32_t beginPos = currPos; 

    //Check first char of this token to decide what to do
    switch (currChar)
    {
        case '+':
            token.kind = TokenKind::PLUS;
            break;
        case '-':
            token.kind = TokenKind::MINUS;
            break;
        case '*':
            token.kind = TokenKind::ASTERISK;
            break;
        case '/':
            token.kind = TokenKind::SLASH;
            break;
        case '=':
            if (peek() == '=')
            {
                token.kind = TokenKind::EQEQ;
                nextChar();
            }
            else
            {
                token.kind = TokenKind::EQ;
            }
            break;

        case '>':
            if (peek() == '=')
            {
                token.kind = TokenKind::GTEQ;
                nextChar();
            }
            else
            {
                token.kind = TokenKind::GT;
            }
            break;
        case '<':
            if (peek() == '=')
            {
                token.kind = TokenKind::LTEQ;
                nextChar();
            }
            else
            {
                token.kind = TokenKind::LT;
            }
            break;

        case '!':
            if (peek() == '=')
            {
                token.kind = TokenKind::NOTEQ;
                nextChar();
            }
            else
            {
                abort(std::string("Invalid token: '!'"));
                return token;
            }
            break;

        case '\"':
            nextChar();
            beginPos += 1;
            while (peek() != '\"' )
            {
                if(currChar == '\r' || currChar == '\n' || currChar == '\t' || currChar == '\\' || currChar == '%')
                {
                    abort("Illegal character in string");
                    return token;
                }
                nextChar();
            }
            nextChar();
            token.kind = TokenKind::STRING;
            token.text = source.substr(beginPos, currPos-beginPos);
            nextChar();
            return token;
        
        case '\n':
            token.kind = TokenKind::NEWLINE;
            break;
        case '\0':
            token.text = "End reached";
            token.kind = TokenKind::EndOfFile;
            return token;
        default:
            if(isdigit(currChar))
            {
                while (isdigit(peek()))
                {
                    nextChar();
                }
                if(peek() == '.')
                {
                    nextChar();
                    if(!isdigit(peek()))
                    {
                        abort("Illegal char in number");
                        return token;
                    }
                    while (isdigit(peek()))
                    {
                        nextChar();
                    }
                }
                token.kind = TokenKind::NUMBER;
            }
            else if (isalpha(currChar))
            {
                while(isalnum(peek()))
                    nextChar();
                
                token.kind = getTokenKind(source.substr(beginPos, currPos-beginPos+1));
            }
            else
            {
                abort(std::string("Invalid token"));
                return token;
            }
            break;
    }

    token.text = source.substr(beginPos, currPos-beginPos+1);
    nextChar();
    return token;
}

TokenKind Lexer::getTokenKind(const std::string& input)
{
    const auto it = keywords.find(input);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenKind::IDENT;
}
