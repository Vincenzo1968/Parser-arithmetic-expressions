// Copyright (C) 2013
// Author: Vincenzo Lo Cicero.
// e-mail: vincenzo.locicero@live.it
//
// This program is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
// You should have received a copy of the GNU General Public License along with
// this program. If not, see http://www.gnu.org/licenses/.

// Questo programma è software free: puoi redistribuirlo e/o modificarlo a piacere
// sotto le condizioni della GNU General Public License così come pubblicata da Free Software
// Foundation, o la versione 3 della licenza o (a propria scelta) una versione successiva. 
// Questo programma è distribuito nella speranza che sia utile, ma SENZA ALCUNA GARANZIA;
// senza nemmeno la garanzia implicita di commerciabilità o idoneità per uno SCOPO PARTICOLARE.
// Vedi la GNU General Public License per maggiori dettagli. 
// Dovresti aver ricevuto una copia della GNU General Public License insieme a questo programma.
// Se questo non è avvenuto, vedi qui: http://www.gnu.org/licenses/.

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

