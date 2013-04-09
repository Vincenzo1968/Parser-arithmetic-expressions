#ifndef LEXER_H
#define LEXER_H

typedef enum tagTokenType
{
	T_EOL,
	T_UNKNOWN,
	T_NUMBER,
	T_OPAREN,
	T_CPAREN,
	T_UMINUS,
	T_UPLUS,	
	T_MULT,
	T_DIV,
	T_PLUS,
	T_MINUS
}TokenTypeEnum;

typedef struct tagToken
{
	TokenTypeEnum Type;
	char str[55];
	double Value;
	int nNextPos;
	TokenTypeEnum PreviousTokenType;
	
}Token;

void initToken(Token *tok);
TokenTypeEnum GetNextToken(const char *str, Token *token);

#endif // LEXER_H
