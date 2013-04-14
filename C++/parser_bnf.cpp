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
#include <cmath>

using namespace std;

#include "parser_bnf.h"

// g++ -Wall -W -O3 -std=c++11 ParseExpr.cpp parser_bnf.cpp lexer.cpp -lm -o expr

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

bool CParser::Parse(const char *strExpr)
{
	bool ret = true;

	strcpy(m_strExpr, strExpr);

	m_Lexer.SetExpr(strExpr);
	m_top = -1;
	m_value = 0;

	m_Lexer.GetNextToken();

	while ( ret && m_Lexer.m_currToken.Type != T_EOL )
	{
		ret = expr();
	}

	if ( m_top >= 0 )
		m_value = m_stack[m_top--];
	m_top = -1;

	return ret;
}

//<expr> = <term> <exprp>	
bool CParser::expr()
{
	if ( !term() )
		return false;
			
	if ( !exprp() )
		return false;
			
	return true;
}

//<exprp> = + <term> <exprp> | - <term> <exprp> | epsilon
bool CParser::exprp()
{
	double right, left;
			
	switch ( m_Lexer.m_currToken.Type )
	{
		case T_PLUS:	
			m_Lexer.GetNextToken();
			if ( !term() )
				return false;
			right = m_stack[m_top--];
			left  = m_stack[m_top--];
			m_stack[++m_top] = left + right;
			if ( !exprp() )
				return false;
			break;
		case T_MINUS:
			m_Lexer.GetNextToken();			
			if ( !term() )
				return false;
			right = m_stack[m_top--];
			left  = m_stack[m_top--];
			m_stack[++m_top] = left - right;
			if ( !exprp() )
				return false;			
			break;
		case T_CPAREN: // epsilon
			break;
		case T_EOL:    // epsilon						
			break;
		default:
			//cout << "Errore di sintassi." << endl;			
			cout << "Syntax error." << endl;
			return false;			
	}
		
	return true;
}	

//<term> = <fact> <termp>
bool CParser::term()
{
	if ( !fact() )
		return false;
		
	if ( !termp() )
		return false;
			
	return true;
}

//<termp> = * <fact> <termp> | / <fact> <termp> | epsilon
bool CParser::termp()
{
	double right, left;
			
	switch ( m_Lexer.m_currToken.Type )
	{
		case T_MULT:	
			m_Lexer.GetNextToken();
			if ( !fact() )
				return false;
			right = m_stack[m_top--];
			left  = m_stack[m_top--];
			m_stack[++m_top] = left * right;
			if ( !termp() )
				return false;
			break;
		case T_DIV:
			m_Lexer.GetNextToken();			
			if ( !fact() )
				return false;
			right = m_stack[m_top--];
			if ( right == 0 )
			{
				//cout << "Errore: divisione per zero." << endl;
				cout << "Error: division by zero." << endl;
				return false;					
			}
			left  = m_stack[m_top--];
			m_stack[++m_top] = left / right;
			if ( !termp() )
				return false;			
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
			//cout << "Errore di sintassi." << endl;
			cout << "Syntax error." << endl;
			return false;							
	}
		
	return true;		
}

//<fact>   = - <expon> | <expon>
bool CParser::fact()
{
	bool bUMinus = false;
		
	if ( m_Lexer.m_currToken.Type == T_UMINUS )
	{
		bUMinus = true;
		m_Lexer.GetNextToken();
	}
			
	if ( !expon() )
		return false;
			
	if ( bUMinus )
		m_stack[m_top] *= -1;
			
	return true;
}	

//<expon>  = <factp> <exponp>
bool CParser::expon()
{
	if ( !factp() )
		return false;
			
	if ( !exponp() )
		return false;
		
	return true;
}

//<exponp> = ^ <factp> <exponp> | epsilon
bool CParser::exponp()
{
	double right, left;
		
	// FOLLOW(exponp) = { / * - + ) $ }
			
	switch ( m_Lexer.m_currToken.Type )
	{
		case T_EXP:	
			m_Lexer.GetNextToken();
			if ( !factp() )
				return false;
			//right = m_stack[m_top--];
			//left  = m_stack[m_top--];
			//m_stack[++m_top] = pow(left, right);
			if ( !exponp() )
				return false;
			// va messo qui perché l'operatore ^ è associativo a destra.
			// placed here because the ^ operator is right associative.
			right = m_stack[m_top--];
			left  = m_stack[m_top--];
			m_stack[++m_top] = pow(left, right);
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
			//cout << "Errore di sintassi." << endl;
			cout << "Syntax error." << endl;
			return false;
	}
		
	return true;
}	

//<factp>  = ( <expr> ) | NUM 
bool CParser::factp()
{
	switch( m_Lexer.m_currToken.Type )
	{
	case T_OPAREN:
		m_Lexer.GetNextToken();
		if ( !expr() )
			return false;
		if ( !match(T_CPAREN) )
		{
			//cout << "Errore: parentesi non bilanciate." << endl;
			cout << "Error: unmatched parentheses." << endl;
			return false;
		}
		break;
	case T_NUMBER:
		m_stack[++m_top] = m_Lexer.m_currToken.Value;
		m_Lexer.GetNextToken();
		break;			
	default:
		//cout << "Errore: atteso numero o parentesi aperta." << endl;
		cout << "Error: expected number or opening parenthesis." << endl;
		return false;
	}

	return true;					
}
