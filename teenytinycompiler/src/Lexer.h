#pragma once
#include <map>
#include <string>
#include <utility>

enum class TokenKind
{
	BadToken = 666,
	EndOfFile = -1,
	NEWLINE = 0,
	NUMBER = 1,
	IDENT = 2,
	STRING = 3,
	//Keywords.
	LABEL = 101,
	GOTO = 102,
	PRINT = 103,
	INPUT = 104,
	LET = 105,
	IF = 106,
	THEN = 107,
	ENDIF = 108,
	WHILE = 109,
	REPEAT = 110,
	ENDWHILE = 111,
	//Operators.
	EQ = 201  ,
	PLUS = 202,
	MINUS = 203,
	ASTERISK = 204,
	SLASH = 205,
	EQEQ = 206,
	NOTEQ = 207,
	LT = 208,
	LTEQ = 209,
	GT = 210,
	GTEQ = 211
};

//Token contains the original text and the type of token.
struct Token
{
	std::string text; 
	TokenKind kind;
	Token(std::string text, TokenKind kind)
		:text(std::move(text)), kind(kind){}
};

class Lexer
{
public:
    //Constructor
    Lexer(const std::string& input);

    //Process the next character.
    void nextChar();

    //Return the lookahead character.
    char peek() const;

    //Invalid token found, print error message and return BadToken.
    static void abort(const std::string& message);
		
    //Skip whitespace except newlines, which we will use to indicate the end of a statement.
	//Consumes the spaces and tabs that we don't care about
    void skipWhitespace();
		
    //Skip comments in the code.
    void skipComment();

    //Return the next token.
	//Classify tokens
    Token getToken();

	//Get TokenKind out of string input
	TokenKind getTokenKind(const std::string& input);
	
	//Getters
	const std::string GetSource() const { return source; }
    const char GetCurrChar() const { return currChar; }
	const int32_t GetCurrPos() const { return currPos; }
	std::map<std::string, TokenKind> GetKeywords() { return keywords; }

private:
	std::string source; //Source code to lex as a string
	char currChar = ' '; //Current character in the string
	int32_t currPos = -1; //Current position in the string
	std::map<std::string, TokenKind> keywords
{{"LABEL", TokenKind::LABEL},{"GOTO", TokenKind::GOTO}, {"PRINT", TokenKind::PRINT} , {"INPUT", TokenKind::INPUT}, {"LET", TokenKind::LET}, {"IF", TokenKind::IF}, {"THEN", TokenKind::THEN}, {"ENDIF", TokenKind::ENDIF}, {"WHILE", TokenKind::WHILE} , {"REPEAT", TokenKind::REPEAT}, {"ENDWHILE", TokenKind::ENDWHILE}};
};



