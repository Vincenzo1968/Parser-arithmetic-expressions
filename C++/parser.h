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

#ifndef PARSER_H
#define PARSER_H

#define MAXSTACK 255

#include "lexer.h"

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

