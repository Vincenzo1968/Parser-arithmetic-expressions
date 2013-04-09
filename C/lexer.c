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

#include <stdio.h>
#include <stdlib.h>  
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "lexer.h"

void initToken(Token *tok)
{
	tok->str[0] = '\0';
	tok->Value = 0;
	tok->Type = tok->PreviousTokenType = T_EOL;	
	tok->nNextPos = 0;	
}

TokenTypeEnum GetNextToken(const char *str, Token *token)
{
	int i;
	char strToken[255];

	while ( 1 )
	{
		while ( str[token->nNextPos++] == ' ' )
			;
		--token->nNextPos;

		if ( str[token->nNextPos] == '\0' )
		{
			token->Type = T_EOL;
			strcpy(token->str, "\n");
			token->nNextPos = 0;
			token->PreviousTokenType = T_EOL;
			return T_EOL;
		}
		else if ( isdigit(str[token->nNextPos]) )
		{
			i = 0;
			while ( isdigit(strToken[i++] = str[token->nNextPos++]) )
				;
			if ( str[token->nNextPos - 1] == '.' )
			{
				while ( isdigit(strToken[i++] = str[token->nNextPos++]) )
					;
				strToken[i - 1] = '\0';
				--token->nNextPos;
				token->Type = T_NUMBER;
				strcpy(token->str, strToken);
				token->Value = atof(strToken);
				token->PreviousTokenType = T_NUMBER;
				return T_NUMBER;
			}
			else
			{
				strToken[i - 1] = '\0';
				--token->nNextPos;
				token->Type = T_NUMBER;
				strcpy(token->str, strToken);
				token->Value = atof(strToken);
				token->PreviousTokenType = T_NUMBER;				
				return T_NUMBER;
			}
		}
		else if ( str[token->nNextPos] == '.' )
		{
			i = 0;
			strToken[i++] = str[token->nNextPos++];
			while ( isdigit(strToken[i++] = str[token->nNextPos++]) )
				;
			strToken[i - 1] = '\0';
			--token->nNextPos;
			token->Type = T_NUMBER;
			strcpy(token->str, strToken);
			token->Value = atof(strToken);
			token->PreviousTokenType = T_NUMBER;			
			return T_NUMBER;
		}
		else if ( str[token->nNextPos] == '(' )
		{
			token->Type = T_OPAREN;
			strcpy(token->str, "(");
			++token->nNextPos;
			token->PreviousTokenType = T_OPAREN;			
			return T_OPAREN;
		}
		else if ( str[token->nNextPos] == ')' )
		{
			token->Type = T_CPAREN;
			strcpy(token->str, ")");
			++token->nNextPos;
			token->PreviousTokenType = T_CPAREN;
			return T_CPAREN;
		}
		else if ( str[token->nNextPos] == '+' )
		{
			strcpy(token->str, "+");
			++token->nNextPos;
			if ( token->PreviousTokenType == T_CPAREN || token->PreviousTokenType == T_NUMBER )
			{
				token->Type = T_PLUS;
				token->PreviousTokenType = T_PLUS;				
				return T_PLUS;
			}
			else
			{
				token->Type = T_UPLUS;
				token->PreviousTokenType = T_UPLUS;
				return T_UPLUS;
			}
		}
		else if ( str[token->nNextPos] == '-' )
		{
			strcpy(token->str, "-");
			++token->nNextPos;
			if ( token->PreviousTokenType == T_CPAREN || token->PreviousTokenType == T_NUMBER )
			{
				token->Type = T_MINUS;
				token->PreviousTokenType = T_MINUS;				
				return T_MINUS;
			}
			else
			{
				token->Type = T_UMINUS;
				token->PreviousTokenType = T_UMINUS;
				return T_UMINUS;
			}
		}
		else if ( str[token->nNextPos] == '*' )
		{
			token->Type = T_MULT;
			strcpy(token->str, "*");
			++token->nNextPos;
			token->PreviousTokenType = T_MULT;			
			return T_MULT;
		}
		else if ( str[token->nNextPos] == '/' )
		{
			token->Type = T_DIV;
			strcpy(token->str, "/");
			++token->nNextPos;
			token->PreviousTokenType = T_DIV;
			return T_DIV;
		}
		/*
		else if ( str[token->nNextPos] == '^' )
		{
			token->Type = T_EXP;
			strcpy(token->str, "^");
			++token->nNextPos;
			token->PreviousTokenType = T_EXP;
			return T_EXP;
		}
		*/
		else
		{
			token->Type = T_UNKNOWN;
			token->str[0] = str[token->nNextPos];
			token->str[1] = '\0';
			++token->nNextPos;
			token->PreviousTokenType = T_UNKNOWN;
			return T_UNKNOWN;
		}
	}

	return T_EOL;
}
