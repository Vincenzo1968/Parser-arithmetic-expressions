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

// gcc -Wall -W -O3 -std=c99 ParseExpr.c parser_bnf.c lexer.c -lm -o expr

#include "parser_bnf.h"

/*
<expr>   = <term> <exprp>
<exprp>  = + <term> <exprp> | - <term> <exprp> | epsilon
<term>   = <fact> <termp>
<termp>  = * <fact> <termp> | / <fact> <termp> | epsilon
<fact>   = - <expon> | <expon>
<expon>  = <factp> <exponp>
<exponp> = ^ <factp> <exponp> | epsilon
<factp>  = ( <expr> ) | NUM 
*/

/*
Nullable Set:
exprp
termp
exponp

First Set:
FIRST(expr)   = { - NUM ( }
FIRST(exprp)  = { - + empty }
FIRST(term)   = { - NUM ( }
FIRST(termp)  = { / * empty }
FIRST(fact)   = { - NUM ( }
FIRST(expon)  = { NUM ( }
FIRST(exponp) = { ^ empty }
FIRST(factp)  = { NUM ( }

Follow Set:
FOLLOW(expr)   = { ) $ }
FOLLOW(exprp)  = { ) $ }
FOLLOW(term)   = { - + ) $ }
FOLLOW(termp)  = { - + ) $ }
FOLLOW(fact)   = { / * - + ) $ }
FOLLOW(expon)  = { / * - + ) $ }
FOLLOW(exponp) = { / * - + ) $ }
FOLLOW(factp)  = { ^ / * - + ) $ }
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


//<expr> = <term> <exprp>	
int expr(ParserData *pd)
{
	if ( !term(pd) )
		return 0;
			
	if ( !exprp(pd) )
		return 0;
			
	return 1;
}

//<exprp> = + <term> <exprp> | - <term> <exprp> | epsilon
int exprp(ParserData *pd)
{
	double right, left;
			
	switch ( pd->m_Token.Type )
	{
		case T_PLUS:	
			GetNextToken(pd->m_strExpr, &(pd->m_Token));
			if ( !term(pd) )
				return 0;
			right = pd->m_stack[pd->m_top--];
			left  = pd->m_stack[pd->m_top--];
			pd->m_stack[++pd->m_top] = left + right;
			if ( !exprp(pd) )
				return 0;
			break;
		case T_MINUS:
			GetNextToken(pd->m_strExpr, &(pd->m_Token));			
			if ( !term(pd) )
				return 0;
			right = pd->m_stack[pd->m_top--];
			left  = pd->m_stack[pd->m_top--];
			pd->m_stack[++pd->m_top] = left - right;
			if ( !exprp(pd) )
				return 0;			
			break;
		case T_CPAREN: // epsilon
			break;
		case T_EOL:    // epsilon						
			break;
		default:
			//printf("Errore di sintassi.\n");			
			printf("Syntax error.\n");
			return 0;			
	}
		
	return 1;
}	

//<term> = <fact> <termp>
int term(ParserData *pd)
{
	if ( !fact(pd) )
		return 0;
		
	if ( !termp(pd) )
		return 0;
			
	return 1;
}

//<termp> = * <fact> <termp> | / <fact> <termp> | epsilon
int termp(ParserData *pd)
{
	double right, left;
			
	switch ( pd->m_Token.Type )
	{
		case T_MULT:	
			GetNextToken(pd->m_strExpr, &(pd->m_Token));
			if ( !fact(pd) )
				return 0;
			right = pd->m_stack[pd->m_top--];
			left  = pd->m_stack[pd->m_top--];
			pd->m_stack[++pd->m_top] = left * right;
			if ( !termp(pd) )
				return 0;
			break;
		case T_DIV:
			GetNextToken(pd->m_strExpr, &(pd->m_Token));			
			if ( !fact(pd) )
				return 0;
			right = pd->m_stack[pd->m_top--];
			if ( right == 0 )
			{
				// printf("Errore: divisione per zero.\n");
				printf("Error: division by zero.\n");
				return 0;
			}
			left  = pd->m_stack[pd->m_top--];
			pd->m_stack[++pd->m_top] = left / right;
			if ( !termp(pd) )
				return 0;			
			break;
		case T_PLUS:   // epsilon
			break;
		case T_MINUS:  // epsilon				
			break;			
		case T_CPAREN: // epsilon
			break;			
		case T_EOL:    // epsilon			
			break;
		default:
			// printf("Errore di sintassi.\n");
			printf("Syntax error.\n");
			return 0;
	}
		
	return 1;
}

//<fact>   = - <expon> | <expon>
int fact(ParserData *pd)
{
	int bUMinus = 0;
		
	if ( pd->m_Token.Type == T_UMINUS )
	{
		bUMinus = 1;
		GetNextToken(pd->m_strExpr, &(pd->m_Token));
	}
			
	if ( !expon(pd) )
		return 0;
			
	if ( bUMinus )
		pd->m_stack[pd->m_top] *= -1;
			
	return 1;
}	

//<expon>  = <factp> <exponp>
int expon(ParserData *pd)
{
	if ( !factp(pd) )
		return 0;
			
	if ( !exponp(pd) )
		return 0;
		
	return 1;
}

//<exponp> = ^ <factp> <exponp> | epsilon
int exponp(ParserData *pd)
{
	double right, left;
		
	// FOLLOW(exponp) = { / * - + ) $ }
			
	switch ( pd->m_Token.Type )
	{
		case T_EXP:	
			GetNextToken(pd->m_strExpr, &(pd->m_Token));
			if ( !factp(pd) )
				return 0;
			//right = pd->m_stack[pd->m_top--];
			//left  = pd->m_stack[pd->m_top--];
			//pd->m_stack[++pd->m_top] = pow(left, right);
			if ( !exponp(pd) )
				return 0;
			// va messo qui perché l'operatore ^ è associativo a destra.
			// placed here because the ^ operator is right associative.
			right = pd->m_stack[pd->m_top--];
			left  = pd->m_stack[pd->m_top--];
			pd->m_stack[++pd->m_top] = pow(left, right);
			break;
		case T_PLUS:   // epsilon
			break;
		case T_MINUS:  // epsilon
			break;				
		case T_MULT:   // epsilon
			break;
		case T_DIV:    // epsilon				
			break;			
		case T_CPAREN: // epsilon
			break;			
		case T_EOL:    // epsilon			
			break;
		default:
			// printf("Errore di sintassi.\n");
			printf("Syntax error.\n");
			return 0;
	}
		
	return 1;
}	

//<factp>  = ( <expr> ) | NUM 
int factp(ParserData *pd)
{
	switch( pd->m_Token.Type )
	{
	case T_OPAREN:
		GetNextToken(pd->m_strExpr, &(pd->m_Token));
		if ( !expr(pd) )
			return 0;
		if ( !match(T_CPAREN, pd) )
		{
			//printf("Errore: parentesi non bilanciate.\n");
			printf("Error: unmatched parentheses.\n");
			return 0;
		}
		break;
	case T_NUMBER:
		pd->m_stack[++pd->m_top] = pd->m_Token.Value;
		GetNextToken(pd->m_strExpr, &(pd->m_Token));
		break;			
	default:
		// printf("Errore: atteso numero o parentesi aperta.\n");;
		printf("Error: expected number or opening parenthesis.\n");
		return 0;
	}

	return 1;
}
