#pragma once
#include <set>
#include <string>
#include "Emitter.h"
#include "Lexer.h"

class Parser
{
public:
    explicit Parser(Lexer& lexer, Emitter& emitter);
    
    //Return true if the current token matches.
    bool checkToken(TokenKind kind) const;
    //Return true if the next token matches.
    bool checkPeek(TokenKind kind) const;
    //Try to match current token. If not, error. Advances the current token.
    void match(TokenKind kind);
    //Advances the current token.
    void nextToken();
    [[noreturn]]
    static void abort(const std::string& message);
    //Parse all statements in program
    void program();
    //Parse statements
    void statement();
    //Handles newlines
    void nl();

    void comparison();
    void expression();
    void term();
    void unary();
    void primary();
    bool isCurrentComparisonOperator() const;

    //Getters
    Lexer* GetLexer() const { return lexer; }
    
private:
    Lexer* lexer;
    Emitter* emitter;
    Token curToken{"Invalid", TokenKind::BadToken};
    Token peekToken{"Invalid", TokenKind::BadToken};

    std::set<std::string> symbols;
    std::set<std::string> labelsDeclared;
    std::set<std::string> labelsGoToed;

};
