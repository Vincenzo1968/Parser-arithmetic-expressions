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


using System;
//using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;

using ExprCS;

/*
expr  : expr1 {('+' | '-') expr1};
expr1 : expr2 {('*' | '/') expr2};
expr2 : ['-'] expr3;
expr3 : expr4 {'^' expr4}
expr4 : T_NUMBER
		| '(' expr ')'
*/

namespace ExprCS
{
    public class CParser
    {
        public CParser()
        {
            m_top = -1;
            m_value = 0;
            m_stack = new double[255];
            m_Lexer = new CLexer();
        }

        public bool Parse(string strExpr)
        {
            bool ret = true;

            m_strExpr = strExpr;
            m_top = -1;
            m_value = 0;

            m_Lexer.SetExpr(strExpr);

            m_Lexer.GetNextToken();

            while (ret && m_Lexer.m_currToken.Type != TokenType.T_EOL)
            {
                ret = expr();
            }

            if (m_top >= 0)
                m_value = m_stack[m_top--];
            m_top = -1;

            return ret;
        }

        public double GetValue()
        {
            return m_value;
        }

		// expr  : expr1 {('+' | '-') expr1};
        private bool expr()
        {
            double right, left;
            TokenType currToken;

            if (!expr1())
                return false;

            while (m_Lexer.m_currToken.Type == TokenType.T_PLUS || m_Lexer.m_currToken.Type == TokenType.T_MINUS)
            {
                currToken = m_Lexer.m_currToken.Type;
                m_Lexer.GetNextToken();

                if (!expr1())
                    return false;

                right = m_stack[m_top--];
                left = m_stack[m_top--];

                if (currToken == TokenType.T_PLUS)
                    m_stack[++m_top] = left + right;
                else if (currToken == TokenType.T_MINUS)
                    m_stack[++m_top] = left - right;

            }

            return true;
        }

		// expr1 : expr2 {('*' | '/') expr2};
        private bool expr1()
        {
            double right, left;
            TokenType currToken;

            if (!expr2())
                return false;

            while (m_Lexer.m_currToken.Type == TokenType.T_MULT || m_Lexer.m_currToken.Type == TokenType.T_DIV)
            {
                currToken = m_Lexer.m_currToken.Type;
                m_Lexer.GetNextToken();

                if (!expr2())
                    return false;

                right = m_stack[m_top--];
                left = m_stack[m_top--];

                if (currToken == TokenType.T_MULT)
                    m_stack[++m_top] = left * right;
                else if (currToken == TokenType.T_DIV)
                {
                    if (right == 0)
                    {
                        //Console.WriteLine("Errore: divisione per zero.");
                        Console.WriteLine("Error: division by zero.");                        
                        return false;
                    }
                    m_stack[++m_top] = left / right;
                }
            }

            return true;
        }

		// expr2 : ['-'] expr3;
        private bool expr2()
        {
            TokenType currToken;
            double dblValue;

            currToken = TokenType.T_EOL;

            if (m_Lexer.m_currToken.Type == TokenType.T_UMINUS)
            {
                currToken = m_Lexer.m_currToken.Type;
                m_Lexer.GetNextToken();
            }

            if (!expr3())
                return false;

            if (currToken == TokenType.T_UMINUS)
            {
                dblValue = m_stack[m_top--];
                dblValue *= -1;
                m_stack[++m_top] = dblValue;
            }

            return true;
        }
        
		// expr3 : expr4 {'^' expr4}
        private bool expr3()
        {
            double right, left;
            //TokenType currToken;
            int count = 0;

            if (!expr4())
                return false;

            while (m_Lexer.m_currToken.Type == TokenType.T_EXP)
            {
				count++;
                //currToken = m_Lexer.m_currToken.Type;
                m_Lexer.GetNextToken();

                if (!expr4())
                    return false;
            }
            
            while ( count )
            {
                right = m_stack[m_top--];
                left = m_stack[m_top--];

                m_stack[++m_top] = Math.Pow(left, right);
                
                count--;
			}

            return true;
        }
        
		// expr4 : T_NUMBER | '(' expr ')'        
        private bool expr4()
        {
            switch (m_Lexer.m_currToken.Type)
            {
                case TokenType.T_NUMBER:
                    m_stack[++m_top] = m_Lexer.m_currToken.Value;
                    m_Lexer.GetNextToken();
                    break;
                case TokenType.T_OPAREN:
                    m_Lexer.GetNextToken();
                    if (!expr())
                        return false;
                    if (!match(TokenType.T_CPAREN))
                    {
                        //Console.WriteLine("Errore: parentesi non bilanciate.");
                        Console.WriteLine("Error: unmatched parentheses.");                        
                        return false;
                    }
                    break;
                default:
                    //Console.WriteLine("Errore: atteso numero, meno unario o parentesi aperta.");
                    //Console.WriteLine("Trovato invece {0}", m_Lexer.m_currToken.str);                    
                    Console.WriteLine("Error: expected number, unary minus or opening parenthesis.");
                    Console.WriteLine("Found instead {0}", m_Lexer.m_currToken.str);
                    
                    return false;
            }

            return true;
        }

        private bool match(TokenType ExpectedToken)
        {
            if (m_Lexer.m_currToken.Type == ExpectedToken)
            {
                m_Lexer.GetNextToken();
                return true;
            }

            return false;
        }

        private CLexer m_Lexer;
        private string m_strExpr;
        private int m_top;
        private double[] m_stack;
        private double m_value;
    }
}

