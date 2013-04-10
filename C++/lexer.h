/*
   Copyright (C) 2013 Vincenzo Lo Cicero

   Author: Vincenzo Lo Cicero.
   e-mail: vincenzo.locicero@live.it

   This file is part of ParseExprLib.

   ParseExprLib is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ParseExprLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with ParseExprLib.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LEXER_H
#define LEXER_H

#include <cstring>

typedef enum tagTokenType
{
	T_EOL,
	T_UNKNOWN,
	T_NUMBER,
	T_OPAREN,
	T_CPAREN,
	T_EXP,	
	T_UMINUS,
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
}Token;

class CLexer
{
public:
	CLexer()
	{
		m_strExpr[0] = '\0';
		m_nNextPos = 0;
		m_PreviousTokenType = T_EOL;
	}

	void SetExpr(const char *strExpr)
	{
		strcpy(m_strExpr, strExpr);
	}

	TokenTypeEnum GetNextToken();

	Token m_currToken;

private:
	char m_strExpr[256];
	int m_nNextPos;
	TokenTypeEnum m_PreviousTokenType;
};

#endif // LEXER_H

