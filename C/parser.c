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
#include <string.h>
#include <math.h>

/* gcc -Wall -W -O3 -std=c99 ParseExpr.c parser.c lexer.c -lm -o expr */

#include "parser.h"

/*
expr  : expr1 {('+' | '-') expr1};
expr1 : expr2 {('*' | '/') expr2};
expr2 : ['-'] expr3;
expr3 : expr4 {'^' expr4}
expr4 : T_NUMBER
		| '(' expr ')'
*/

int Parse(const char *strExpr, double *dblRet)
{
	int ret = 1;	
	ParserData md;
	
	*dblRet = 0;

	strcpy(md.m_strExpr, strExpr);
	md.m_top = -1;
	md.m_value = 0;

	initToken(&(md.m_Token));
	GetNextToken(md.m_strExpr, &(md.m_Token));

	while ( ret && md.m_Token.Type != T_EOL )
	{
		ret = expr(&md);
	}

	if ( md.m_top >= 0 )
		md.m_value = md.m_stack[md.m_top--];
	md.m_top = -1;
	
	*dblRet = md.m_value;

	return ret;
}

int match(TokenTypeEnum ExpectedToken, ParserData *pd)
{
	if ( pd->m_Token.Type == ExpectedToken )
	{
		GetNextToken(pd->m_strExpr, &(pd->m_Token));		
		return 1;
	}

	return 0;
}

/* expr  : expr1 {('+' | '-') expr1}; */
int expr(ParserData *pd)
{
	double right, left;
	int currToken;

	if ( !expr1(pd) )
		return 0;

	while ( pd->m_Token.Type == T_PLUS || pd->m_Token.Type == T_MINUS )
	{
		currToken = pd->m_Token.Type;
		GetNextToken(pd->m_strExpr, &(pd->m_Token));

		if ( !expr1(pd) )
			return 0;

		right = pd->m_stack[pd->m_top--];
		left  = pd->m_stack[pd->m_top--];

		if ( currToken == T_PLUS )
		{
			pd->m_stack[++pd->m_top] = left + right;
		}
		else if ( currToken == T_MINUS )
		{
			pd->m_stack[++pd->m_top] = left - right;
		}
	}

	return 1;
}

/* expr1 : expr2 {('*' | '/') expr2}; */
int expr1(ParserData *pd)
{
	double right, left;
	int currToken;

	if ( !expr2(pd) )
		return 0;

	while ( pd->m_Token.Type == T_MULT || pd->m_Token.Type == T_DIV )
	{
		currToken = pd->m_Token.Type;
		GetNextToken(pd->m_strExpr, &(pd->m_Token));

		if ( !expr2(pd) )
			return 0;

		right = pd->m_stack[pd->m_top--];
		left  = pd->m_stack[pd->m_top--];
		
		if ( currToken == T_MULT )
		{
			pd->m_stack[++pd->m_top] = left * right;
		}
		else if ( currToken == T_DIV )
		{
			if ( right == 0 )
			{
				printf("Error: division by zero.\n");
				return 0;
			}
			pd->m_stack[++pd->m_top] = left / right;
		}
	}

	return 1;
}

/* expr2 : ['-'] expr3; */
int expr2(ParserData *pd)
{
	int currToken;
	double dblValue;

	currToken = pd->m_Token.Type;
	if ( pd->m_Token.Type == T_UMINUS )
	{
		GetNextToken(pd->m_strExpr, &(pd->m_Token));		
	}

	if ( !expr3(pd) )
		return 0;

	if ( currToken == T_UMINUS )
	{
		dblValue = pd->m_stack[pd->m_top--];
		dblValue *= -1;
		pd->m_stack[++pd->m_top] = dblValue;
	}

	return 1;
}

/* expr3 : expr4 {'^' expr4} */
int expr3(ParserData *pd)
{
	double right, left;
	int count = 0;

	if ( !expr4(pd) )
		return 0;

	while ( pd->m_Token.Type == T_EXP )
	{
		count++;
		
		GetNextToken(pd->m_strExpr, &(pd->m_Token));

		if ( !expr4(pd) )
			return 0;
	}
	
	while ( count )
	{
		right = pd->m_stack[pd->m_top--];
		left  = pd->m_stack[pd->m_top--];
		
		pd->m_stack[++pd->m_top] = pow(left, right);
		
		count--;
	}	

	return 1;
}

/* expr4 : T_NUMBER | '(' expr ')' */
int expr4(ParserData *pd)
{
	switch( pd->m_Token.Type )
	{
	case T_NUMBER:
		pd->m_stack[++pd->m_top] = pd->m_Token.Value;
		GetNextToken(pd->m_strExpr, &(pd->m_Token));		
		break;
	case T_OPAREN:
		GetNextToken(pd->m_strExpr, &(pd->m_Token));		
		if ( !expr(pd) )
			return 0;
		if ( !match(T_CPAREN, pd) )
		{
			printf("Error: unmatched parentheses.\n");
			return 0;
		}
		break;
	default:
		printf("Error: expected number, unary minus or opening parenthesis.\n");
		printf("Found instead %s.\n", pd->m_Token.str);
		return 0;
	}

	return 1;
}
