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

import java.lang.String;
import java.lang.StringBuilder;

/*
<start> = <expr>
<expr>  = <term> <exprp>
<exprp> = + <term> <exprp> | - <term> <exprp> | epsilon
<term>  = <fact> <termp>
<termp> = * <fact> <termp> | / <fact> <termp> | epsilon
<fact>  = - <factp> | <factp>
<factp> = ( <expr> ) | NUM 
*/

/*
Nullable Set:
exprp
termp

First Set:
FIRST(start) = { - NUM ( }
FIRST(expr) = { - NUM ( }
FIRST(exprp) = { - + empty }
FIRST(term) = { - NUM ( }
FIRST(termp) = { / * empty }
FIRST(fact) = { - NUM ( }
FIRST(factp) = { NUM ( }

Follow Set:
FOLLOW(start) = { $ }
FOLLOW(expr) = { ) $ }
FOLLOW(exprp) = { ) $ }
FOLLOW(term) = { - + ) $ }
FOLLOW(termp) = { - + ) $ }
FOLLOW(fact) = { / * - + ) $ }
FOLLOW(factp) = { / * - + ) $ }
*/

public class CParser
{
	public CParser()
	{
		m_Lexer = new CLexer();	
		m_top = -1;
		m_value = 0;
		m_stack = new double[255];
	}

	public boolean Parse(String strExpr)
	{
		boolean ret = true;

		m_strExpr = new String(strExpr);
		m_top = -1;
		m_value = 0;		

		m_Lexer.SetExpr(strExpr);

		m_Lexer.GetNextToken();

		while ( ret && m_Lexer.m_currToken.Type != CLexer.TokenTypeEnum.T_EOL )
		{
			ret = start();
		}

		if ( m_top >= 0 )
			m_value = m_stack[m_top--];
		m_top = -1;

		return ret;	
	}
	
	public double GetValue()
	{
		return m_value;
	}
	

	//<start> = <expr>
	private boolean start()
	{
		return expr();
	}

	//<expr> = <term> <exprp>	
	private boolean expr()
	{
		if ( !term() )
			return false;
			
		if ( !exprp() )
			return false;
			
		return true;
	}
	
	//<exprp> = + <term> <exprp> | - <term> <exprp> | epsilon
	private boolean exprp()
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
				System.out.println("Errore di sintassi.");
				return false;			
		}
		
		return true;
	}	

	//<term> = <fact> <termp>
	private boolean term()
	{
		if ( !fact() )
			return false;
			
		if ( !termp() )
			return false;
			
		return true;
	}
	
	//<termp> = * <fact> <termp> | / <fact> <termp> | epsilon
	private boolean termp()
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
					System.out.println("Errore: divisione per zero.");
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
				System.out.println("Errore di sintassi.");
				return false;							
		}
		
		return true;		
	}
	
	//<fact>  = - <factp> | <factp>
	private boolean fact()
	{
		boolean bUMinus = false;
		
		if ( m_Lexer.m_currToken.Type == CLexer.TokenTypeEnum.T_UMINUS )
		{
			bUMinus = true;
			m_Lexer.GetNextToken();
		}
			
		if ( !factp() )
			return false;
			
		if ( bUMinus )
			m_stack[m_top] *= -1;
			
		return true;
	}	
	
	//<factp> = ( <expr> ) | NUM 
	private boolean factp()
	{
		switch( m_Lexer.m_currToken.Type )
		{
		case T_OPAREN:
			m_Lexer.GetNextToken();
			if ( !expr() )
				return false;
			if ( !match(CLexer.TokenTypeEnum.T_CPAREN) )
			{
				System.out.println("Errore: parentesi non bilanciate.");
				return false;
			}
			break;
		case T_NUMBER:
			m_stack[++m_top] = m_Lexer.m_currToken.Value;
			m_Lexer.GetNextToken();
			break;			
		default:
			System.out.println("Errore: atteso numero o parentesi aperta.");
			return false;
		}

		return true;					
	}
	
	private boolean match(CLexer.TokenTypeEnum ExpectedToken)
	{
		if ( m_Lexer.m_currToken.Type == ExpectedToken )
		{
			m_Lexer.GetNextToken();
			return true;
		}

		return false;
	}
	
	private CLexer m_Lexer;
	private String m_strExpr;	
	private int m_top;              
	private double[] m_stack;
	private double m_value;
}
