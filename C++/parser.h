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

#ifndef PARSER_H
#define PARSER_H

#define MAXSTACK 255

#include "Lexer.h"

class CParser
{
public:
	CParser()
	{
		m_strExpr[0] = '\0';
		m_top = -1;
		m_value = 0;
	}

	bool Parse(const char *strExpr);
	double GetValue()
	{
		return m_value;
	}

private:
	bool expr();
	bool expr1();
	bool expr2();
	bool expr3();

	bool match(TokenTypeEnum ExpectedToken)
	{
		if ( m_Lexer.m_currToken.Type == ExpectedToken )
		{
			m_Lexer.GetNextToken();
			return true;
		}

		return false;
	}

	CLexer m_Lexer;
	char m_strExpr[256];
	int m_top;              
	double m_stack[MAXSTACK];
	double m_value;
};


#endif // PARSER_H

