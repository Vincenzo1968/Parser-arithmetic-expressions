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

#include <iostream>
#include <cstdlib>

#include "lexer.h"

TokenTypeEnum CLexer::GetNextToken()
{
	int i;
	char strToken[255];

	while ( 1 )
	{
		while ( m_strExpr[m_nNextPos++] == ' ' )
			;
		--m_nNextPos;

		if ( m_strExpr[m_nNextPos] == '\0' )
		{
			m_currToken.Type = T_EOL;
			strcpy(m_currToken.str, "\n");
			m_nNextPos = 0;
			m_PreviousTokenType = T_EOL;
			return T_EOL;
		}
		else if ( isdigit(m_strExpr[m_nNextPos]) )
		{
			i = 0;
			while ( isdigit(strToken[i++] = m_strExpr[m_nNextPos++]) )
				;
			if ( m_strExpr[m_nNextPos - 1] == '.' )
			{
				while ( isdigit(strToken[i++] = m_strExpr[m_nNextPos++]) )
					;
				strToken[i - 1] = '\0';
				--m_nNextPos;
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = T_NUMBER;
				strcpy(m_currToken.str, strToken);
				m_currToken.Value = atof(strToken);
				return T_NUMBER;
			}
			else
			{
				strToken[i - 1] = '\0';
				--m_nNextPos;
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = T_NUMBER;
				strcpy(m_currToken.str, strToken);
				m_currToken.Value = atof(strToken);
				return T_NUMBER;
			}
		}
		else if ( m_strExpr[m_nNextPos] == '.' )
		{
			i = 0;
			strToken[i++] = m_strExpr[m_nNextPos++];
			while ( isdigit(strToken[i++] = m_strExpr[m_nNextPos++]) )
				;
			strToken[i - 1] = '\0';
			--m_nNextPos;
			m_PreviousTokenType = m_currToken.Type;
			m_currToken.Type = T_NUMBER;
			strcpy(m_currToken.str, strToken);
			m_currToken.Value = atof(strToken);
			return T_NUMBER;
		}
		else if ( m_strExpr[m_nNextPos] == '(' )
		{
			m_PreviousTokenType = m_currToken.Type;
			m_currToken.Type = T_OPAREN;
			strcpy(m_currToken.str, "(");
			++m_nNextPos;
			return T_OPAREN;
		}
		else if ( m_strExpr[m_nNextPos] == ')' )
		{
			m_PreviousTokenType = m_currToken.Type;
			m_currToken.Type = T_CPAREN;
			strcpy(m_currToken.str, ")");
			++m_nNextPos;
			return T_CPAREN;
		}
		else if ( m_strExpr[m_nNextPos] == '+' )
		{
			m_PreviousTokenType = m_currToken.Type;
			strcpy(m_currToken.str, "+");
			++m_nNextPos;
			m_currToken.Type = T_PLUS;
			return T_PLUS;
		}
		else if ( m_strExpr[m_nNextPos] == '-' )
		{
			strcpy(m_currToken.str, "-");
			++m_nNextPos;
			m_PreviousTokenType = m_currToken.Type;
			if ( m_PreviousTokenType == T_CPAREN || m_PreviousTokenType == T_NUMBER )
			{
				m_currToken.Type = T_MINUS;
				return T_MINUS;
			}
			else
			{
				m_currToken.Type = T_UMINUS;
				return T_UMINUS;
			}
		}
		else if ( m_strExpr[m_nNextPos] == '*' )
		{
			m_PreviousTokenType = m_currToken.Type;
			m_currToken.Type = T_MULT;
			strcpy(m_currToken.str, "*");
			++m_nNextPos;
			return T_MULT;
		}
		else if ( m_strExpr[m_nNextPos] == '/' )
		{
			m_PreviousTokenType = m_currToken.Type;
			m_currToken.Type = T_DIV;
			strcpy(m_currToken.str, "/");
			++m_nNextPos;
			return T_DIV;
		}
		else
		{
			m_PreviousTokenType = m_currToken.Type;
			m_currToken.Type = T_UNKNOWN;
			m_currToken.str[0] = m_strExpr[m_nNextPos];
			m_currToken.str[1] = '\0';
			++m_nNextPos;
			return T_UNKNOWN;
		}
	}

	return T_EOL;
}

