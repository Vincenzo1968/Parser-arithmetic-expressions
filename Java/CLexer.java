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

public class CLexer
{
	static public enum TokenTypeEnum
	{
		T_EOL,
		T_UNKNOWN,
		T_NUMBER,
		T_OPAREN,
		T_CPAREN,
		T_UMINUS,
		T_MULT,
		T_DIV,
		T_PLUS,
		T_MINUS,
		T_EXP		
	}

	static public class Token
	{
		TokenTypeEnum Type;
		String str;
		double Value;
	}
	public CLexer()
	{
		m_nNextPos = 0;
		m_PreviousTokenType = TokenTypeEnum.T_EOL;
		m_currToken = new Token();
	}

	public void SetExpr(String str)
	{
		m_strExpr = new String(str);
	}

	public TokenTypeEnum GetNextToken()
	{	
		StringBuilder strToken = new StringBuilder();
		
		StringBuilder str = new StringBuilder();		
		str.append(m_strExpr);	
				
		if ( m_nNextPos >= str.length() )						
		{
			m_currToken = new Token();			
			m_currToken.Type = TokenTypeEnum.T_EOL;
			m_currToken.str = new String("EOL");
			m_nNextPos = 0;
			m_PreviousTokenType = TokenTypeEnum.T_EOL;
			return TokenTypeEnum.T_EOL;
		}		
			
		while ( true )
		{
			while ( m_nNextPos < str.length() && str.charAt(m_nNextPos++) == ' ' )
				;
			--m_nNextPos;

			if ( m_nNextPos >= str.length() )						
			{
				m_currToken = new Token();						
				m_currToken.Type = TokenTypeEnum.T_EOL;
				m_currToken.str = new String("EOL");
				m_nNextPos = 0;
				m_PreviousTokenType = TokenTypeEnum.T_EOL;
				return TokenTypeEnum.T_EOL;
			}
			else if ( isdigit(str.charAt(m_nNextPos)) )					
			{			
				while ( m_nNextPos < str.length() && isdigit(str.charAt(m_nNextPos)) )
				{
					strToken.append(str.charAt(m_nNextPos));					
					m_nNextPos++;
				}
				if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '.' )				
				{
					str.setCharAt(m_nNextPos, '.');
					m_nNextPos++;					
					while ( m_nNextPos < str.length() && isdigit(str.charAt(m_nNextPos)) )
					{
						strToken.append(str.charAt(m_nNextPos));
						m_nNextPos++;						
					}
					m_PreviousTokenType = m_currToken.Type;
					m_currToken.Type = TokenTypeEnum.T_NUMBER;
					m_currToken.str = new String(strToken.toString());
					m_currToken.Value = Double.valueOf(m_currToken.str).doubleValue();
					return TokenTypeEnum.T_NUMBER;
				}
				else
				{
					m_PreviousTokenType = m_currToken.Type;
					m_currToken.Type = TokenTypeEnum.T_NUMBER;
					m_currToken.str = new String(strToken.toString());				
					m_currToken.Value = Double.valueOf(m_currToken.str).doubleValue();				
					return TokenTypeEnum.T_NUMBER;
				}
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '.' )
			{			
				str.setCharAt(m_nNextPos, '.');
				m_nNextPos++;				
				while ( m_nNextPos < str.length() && isdigit(str.charAt(m_nNextPos)) )
				{
					strToken.append(str.charAt(m_nNextPos));
					m_nNextPos++;					
				}					
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_NUMBER;
				m_currToken.str = new String(strToken);			
				m_currToken.Value = Double.valueOf(m_currToken.str).doubleValue();				
				return TokenTypeEnum.T_NUMBER;
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '(' )
			{			
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_OPAREN;
				m_currToken.str = new String("(");							
				++m_nNextPos;
				return TokenTypeEnum.T_OPAREN;
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == ')' )
			{
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_CPAREN;
				m_currToken.str = new String(")");							
				++m_nNextPos;
				return TokenTypeEnum.T_CPAREN;
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '+' )
			{
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.str = new String("+");							
				++m_nNextPos;
				m_currToken.Type = TokenTypeEnum.T_PLUS;
				return TokenTypeEnum.T_PLUS;
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '-' )
			{
				m_currToken.str = new String("-");
				++m_nNextPos;
				m_PreviousTokenType = m_currToken.Type;
				if ( m_PreviousTokenType == TokenTypeEnum.T_CPAREN || 
					m_PreviousTokenType == TokenTypeEnum.T_NUMBER )
				{
					m_currToken.Type = TokenTypeEnum.T_MINUS;
					return TokenTypeEnum.T_MINUS;
				}
				else
				{
					m_currToken.Type = TokenTypeEnum.T_UMINUS;
					return TokenTypeEnum.T_UMINUS;
				}
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '*' )
			{
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_MULT;
				m_currToken.str = new String("*");
				++m_nNextPos;
				return TokenTypeEnum.T_MULT;
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '/' )
			{
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_DIV;
				m_currToken.str = new String("/");
				++m_nNextPos;
				return TokenTypeEnum.T_DIV;
			}
			else if ( m_nNextPos < str.length() && str.charAt(m_nNextPos) == '^' )
			{
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_EXP;
				m_currToken.str = new String("^");
				++m_nNextPos;
				return TokenTypeEnum.T_EXP;
			}			
			else
			{
				m_PreviousTokenType = m_currToken.Type;
				m_currToken.Type = TokenTypeEnum.T_UNKNOWN;
				m_currToken.str = new String(str);								
				++m_nNextPos;
				return TokenTypeEnum.T_UNKNOWN;
			}
		}
	}
	
	private boolean isdigit(char c)
	{
		return (c >= '0' && c <= '9');
	}

	public Token m_currToken;
	private String m_strExpr;
	private int m_nNextPos;
	private TokenTypeEnum m_PreviousTokenType;
}

