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

#include <iostream>
#include <cstdlib>
using namespace std;

#include "parser.h"

/*
expr  : expr1 {('+' | '-') expr1};
expr1 : expr2 {('*' | '/') expr2};
expr2 : ['-'] expr3;
expr3 : T_NUMBER
		| '(' expr ')'
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


//expr  : expr1 {('+' | '-') expr1};
bool CParser::expr()
{
	double right, left;
	int currToken;

	if ( !expr1() )
		return false;

	while ( m_Lexer.m_currToken.Type == T_PLUS || m_Lexer.m_currToken.Type == T_MINUS )
	{
		currToken = m_Lexer.m_currToken.Type;
		m_Lexer.GetNextToken();

		if ( !expr1() )
			return false;

		right = m_stack[m_top--];
		left  = m_stack[m_top--];

		if ( currToken == T_PLUS )
		{
			m_stack[++m_top] = left + right;
		}
		else if ( currToken == T_MINUS )
		{
			m_stack[++m_top] = left - right;
		}
	}

	return true;
}

//expr1 : expr2 {('*' | '/') expr2};
bool CParser::expr1()
{
	double right, left;
	int currToken;

	if ( !expr2() )
		return false;

	while ( m_Lexer.m_currToken.Type == T_MULT || m_Lexer.m_currToken.Type == T_DIV )
	{
		currToken = m_Lexer.m_currToken.Type;
		m_Lexer.GetNextToken();

		if ( !expr2() )
			return false;

		right = m_stack[m_top--];
		left  = m_stack[m_top--];
		
		if ( currToken == T_MULT )
		{
			m_stack[++m_top] = left * right;
		}
		else if ( currToken == T_DIV )
		{
			if ( right == 0 )
			{
				//cout << "Errore: divisione per zero." << endl;
				cout << "Error: division by zero." << endl;				
				return false;
			}
			m_stack[++m_top] = left / right;
		}
	}

	return true;
}

//expr2 : ['-'] expr3;
bool CParser::expr2()
{
	int currToken;
	double dblValue;

	currToken = m_Lexer.m_currToken.Type;
	if ( m_Lexer.m_currToken.Type == T_UMINUS )
	{
		m_Lexer.GetNextToken();
	}

	if ( !expr3() )
		return false;

	if ( currToken == T_UMINUS )
	{
		dblValue = m_stack[m_top--];
		dblValue *= -1;
		m_stack[++m_top] = dblValue;
	}

	return true;
}

//expr3 : T_NUMBER
//		| '(' expr ')'
bool CParser::expr3()
{
	switch( m_Lexer.m_currToken.Type )
	{
	case T_NUMBER:
		m_stack[++m_top] = m_Lexer.m_currToken.Value;
		m_Lexer.GetNextToken();
		break;
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
	default:
		//cout << "Errore: atteso numero, meno unario o parentesi aperta." << endl;
		//cout << "Trovato invece " << m_Lexer.m_currToken.str << endl;		
		cout << "Error: expected number, unary minus or opening parenthesis." << endl;				
		cout << "Found instead " << m_Lexer.m_currToken.str << endl;
		return false;
	}

	return true;
}

