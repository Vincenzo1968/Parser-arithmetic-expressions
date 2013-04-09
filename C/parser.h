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

#include "lexer.h"

#define MAXSTACK 255

typedef struct tagParserData
{
	char m_strExpr[255];
	Token m_Token;
	int m_top;              
	double m_stack[MAXSTACK];
	double m_value;	
} ParserData;


int Parse(const char *strExpr, double *dblRet);

int match(TokenTypeEnum ExpectedToken, ParserData *pd);

int expr(ParserData *pd);
int expr1(ParserData *pd);
int expr2(ParserData *pd);
int expr3(ParserData *pd);

#endif // PARSER_H
