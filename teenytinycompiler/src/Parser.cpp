#include "Parser.h"
#include <iostream>
#include <ostream>

Parser::Parser(Lexer& lexer, Emitter& emitter)
    :lexer(&lexer), emitter(&emitter)
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
    emitter->emitHeaderLine("#include <stdio.h>");
    emitter->emitHeaderLine("int main(void){");

    while(checkToken(TokenKind::NEWLINE))
        nextToken();
    
    while(!checkToken(TokenKind::EndOfFile))
    {
        statement();
    }

    emitter->emitLine("return 0;");
    emitter->emitLine("}");

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
        {
            emitter->emit("printf(\"" + curToken.text + "\\n\");");
            nextToken();
        }
        else
        {
            emitter->emit(std::string("printf(\"%") + ".2f\\n\", (float)(");
        	expression();
            emitter->emitLine("));");
        }
    }
    else if(checkToken(TokenKind::IF))
    {
        std::cout << "STATEMENT-IF\n";
        nextToken();
        emitter->emit("if(");
        comparison();

        match(TokenKind::THEN);
        nl();
        emitter->emitLine("){");

        while(!checkToken(TokenKind::ENDIF))
        {
            statement();
        }

        match(TokenKind::ENDIF);
        emitter->emitLine("}");
        
    }
    else if(checkToken(TokenKind::WHILE))
    {
        std::cout << "STATEMENT-WHILE\n";
        nextToken();
        emitter->emit("while(");
        comparison();
        match(TokenKind::REPEAT);
        nl();
        emitter->emitLine("){");
        
        while (!checkToken(TokenKind::ENDWHILE))
        {
            statement();
        }
        match(TokenKind::ENDWHILE);
        emitter->emitLine("}");
    }
    else if(checkToken(TokenKind::LABEL))
    {
        std::cout << "STATEMENT-LABEL\n";
        nextToken();

        if (labelsDeclared.find(curToken.text) != labelsDeclared.end())
            abort("Label already exists: " + curToken.text);

        labelsDeclared.insert(curToken.text);

        emitter->emitLine(curToken.text + ":");
        match(TokenKind::IDENT);
    }
    else if(checkToken(TokenKind::GOTO))
    {
        std::cout << "STATEMENT-GOTO\n";
        nextToken();
        labelsGoToed.insert(curToken.text);
        emitter->emitLine("goto " + curToken.text + ";");
        match(TokenKind::IDENT);
    }
    else if(checkToken(TokenKind::LET))
    {
        std::cout << "STATEMENT-LET\n";
        nextToken();

        if(symbols.find(curToken.text) == symbols.end())
        {
            symbols.insert(curToken.text);
            emitter->emitHeaderLine("float " + curToken.text + ";");
        }

        emitter->emit(curToken.text + " = ");

        match(TokenKind::IDENT);
        match(TokenKind::EQ);
        expression();
        emitter->emitLine(";");
    }
    else if(checkToken(TokenKind::INPUT))
    {
        std::cout << "STATEMENT-INPUT\n";
        nextToken();
        if (symbols.find(curToken.text) == symbols.end())
        {
            symbols.insert(curToken.text);
            emitter->emitHeaderLine("float " + curToken.text + ";");
        }
        emitter->emitLine(std::string("if(0 == scanf(\"%") + "f\", &" + curToken.text + ")) {");
        emitter->emitLine(curToken.text + " = 0;");
        emitter->emit("scanf(\"%");
        emitter->emitLine("*s\");");
        emitter->emitLine("}");

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
        emitter->emit(curToken.text);
        nextToken();
        expression();
    }
    else
    {
        abort("Expected comparison operator at: " + curToken.text);
    }

    while(isCurrentComparisonOperator())
    {
        emitter->emit(curToken.text);
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
        emitter->emit(curToken.text);
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
        emitter->emit(curToken.text);
        nextToken();
        unary();
    }
}

void Parser::unary()
{
    std::cout << "UNARY\n";
    if(checkToken(TokenKind::PLUS) or checkToken(TokenKind::MINUS))
    {
        emitter->emit(curToken.text);
        nextToken();
    }
    primary();
}

void Parser::primary()
{
    std::cout << "PRIMARY (" << curToken.text << ")\n";

	if(checkToken(TokenKind::NUMBER))
	{
        emitter->emit(curToken.text);
	    nextToken();
	}

    else if(checkToken(TokenKind::IDENT))
    {
        if (symbols.find(curToken.text) == symbols.end())
        {
            abort("Referencing variable before assignment: " + curToken.text);
        }
        emitter->emit(curToken.text);
        nextToken();
    }
    else
	    abort("Unexpected token at" + curToken.text);
}

bool Parser::isCurrentComparisonOperator() const
{
    return checkToken(TokenKind::GT) || checkToken(TokenKind::GTEQ) || checkToken(TokenKind::LT) || checkToken(TokenKind::LTEQ) || checkToken(TokenKind::EQEQ) || checkToken(TokenKind::NOTEQ);
}
