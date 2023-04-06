#include "Parser.h"
#include <iostream>
#include <ostream>

Parser::Parser(Lexer& lexer)
    :lexer(&lexer)
{
    nextToken();
    nextToken();
}

bool Parser::checkToken(const TokenKind kind) const
{
    return kind == curToken.kind;
}

bool Parser::checkPeek(const TokenKind kind) const
{
    return kind == peekToken.kind;
}

void Parser::match(const TokenKind kind)
{
    if(!checkToken(kind))
        abort(std::string("Expected different token"));
    
    nextToken();
}

void Parser::nextToken()
{
    curToken = peekToken;
    peekToken = lexer->getToken();
}

[[noreturn]]
void Parser::abort(const std::string& message)
{
    std::cout << "Error! " << message << std::endl;
    exit(1);
}

void Parser::program()
{
    std::cout << "PROGRAM\n";

    while(checkToken(TokenKind::NEWLINE))
        nextToken();
    
    while(!checkToken(TokenKind::EndOfFile))
    {
        statement();
    }

    for(const auto& labelGoTo : labelsGoToed)
    {
        //If label to go to is not in declared labels --> abort
        if (labelsDeclared.find(labelGoTo) == labelsDeclared.end())
        {
            abort("Attempting to GOTO undeclared label:" + labelGoTo);
        }
    }
}

void Parser::statement()
{
    if(checkToken(TokenKind::PRINT))
    {
        std::cout << "STATEMENT-PRINT\n";
        nextToken();
        if (checkToken(TokenKind::STRING))
            nextToken();
        else
            expression();
    }
    else if(checkToken(TokenKind::IF))
    {
        std::cout << "STATEMENT-IF\n";
        nextToken();
        comparison();

        match(TokenKind::THEN);
        nl();

        while(!checkToken(TokenKind::ENDIF))
        {
            statement();
        }

        match(TokenKind::ENDIF);
        
    }
    else if(checkToken(TokenKind::WHILE))
    {
        std::cout << "STATEMENT-WHILE\n";
        nextToken();
        comparison();
        match(TokenKind::REPEAT);
        nl();
        
        while (!checkToken(TokenKind::ENDWHILE))
        {
            statement();
        }
        match(TokenKind::ENDWHILE);
    }
    else if(checkToken(TokenKind::LABEL))
    {
        std::cout << "STATEMENT-LABEL\n";
        nextToken();

        if (labelsDeclared.find(curToken.text) != labelsDeclared.end())
            abort("Label already exists: " + curToken.text);

        labelsDeclared.insert(curToken.text);
        match(TokenKind::IDENT);
    }
    else if(checkToken(TokenKind::GOTO))
    {
        std::cout << "STATEMENT-GOTO\n";
        nextToken();
        labelsGoToed.insert(curToken.text);
        match(TokenKind::IDENT);
    }
    else if(checkToken(TokenKind::LET))
    {
        std::cout << "STATEMENT-LET\n";
        nextToken();

        if(symbols.find(curToken.text) == symbols.end())
        {
            symbols.insert(curToken.text);
        }

        match(TokenKind::IDENT);
        match(TokenKind::EQ);
        expression();
        
    }
    else if(checkToken(TokenKind::INPUT))
    {
        std::cout << "STATEMENT-INPUT\n";
        nextToken();
        if (symbols.find(curToken.text) == symbols.end())
        {
            symbols.insert(curToken.text);
        }
        match(TokenKind::IDENT);
    }
    else
    {
        abort(std::string("Invalid statement at ") + curToken.text);
    }
    nl();
}

void Parser::nl()
{
    std::cout << "NEWLINE\n";
    match(TokenKind::NEWLINE);
    while(checkToken(TokenKind::NEWLINE))
        nextToken();
}

void Parser::comparison()
{
    std::cout << "COMPARISON\n";
    expression();
    if(isCurrentComparisonOperator())
    {
        nextToken();
        expression();
    }
    else
    {
        abort("Expected comparison operator at: " + curToken.text);
    }

    while(isCurrentComparisonOperator())
    {
        nextToken();
        expression();
    }
}

void Parser::expression()
{
    std::cout << "EXPRESSION\n";
    term();
    while (checkToken(TokenKind::PLUS) || checkToken(TokenKind::MINUS))
    {
        nextToken();
        term();
    }
}

void Parser::term()
{
    std::cout << "TERM\n";
    unary();
    while(checkToken(TokenKind::ASTERISK) || checkToken(TokenKind::SLASH))
    {
        nextToken();
        unary();
    }
}

void Parser::unary()
{
    std::cout << "UNARY\n";
    if(checkToken(TokenKind::PLUS) or checkToken(TokenKind::MINUS))
    {
        nextToken();
    }
    primary();
}

void Parser::primary()
{
    std::cout << "PRIMARY (" << curToken.text << ")\n";

	if(checkToken(TokenKind::NUMBER))
	    nextToken();

    else if(checkToken(TokenKind::IDENT))
    {
        if (symbols.find(curToken.text) == symbols.end())
        {
            abort("Referencing variable before assignment: " + curToken.text);
        }
        nextToken();
    }
    else
	    abort("Unexpected token at" + curToken.text);
}

bool Parser::isCurrentComparisonOperator() const
{
    return checkToken(TokenKind::GT) || checkToken(TokenKind::GTEQ) || checkToken(TokenKind::LT) || checkToken(TokenKind::LTEQ) || checkToken(TokenKind::EQEQ) || checkToken(TokenKind::NOTEQ);
}
