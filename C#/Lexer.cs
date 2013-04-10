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

namespace ExprCS
{
    public enum TokenType
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

    public struct Token
    {
        public TokenType Type;
        public string str;
        public double Value;
    }

    public class CLexer
    {
        public CLexer()
        {
            m_strExpr = "";
            m_nNextPos = 0;
            m_PreviousTokenType = TokenType.T_EOL;
            m_currToken = new Token();
        }

        public void SetExpr(string strExpr)
        {
            m_strExpr = strExpr;
        }

        private bool isdigit(char c)
        {
            return (c >= '0' && c <= '9');
        }

        public TokenType GetNextToken()
        {
            StringBuilder strToken = new StringBuilder("", 256);

            if (m_nNextPos >= m_strExpr.Length)
            {
                m_currToken.Type = TokenType.T_EOL;
                m_currToken.str = "EOL";
                m_nNextPos = 0;
                m_PreviousTokenType = TokenType.T_EOL;
                return TokenType.T_EOL;
            }

            char[] strExpr = new char[1024];
            strExpr = m_strExpr.ToCharArray();

            while (true)
            {
                while (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos++] == ' ')
                    ;
                --m_nNextPos;

                if (m_nNextPos >= m_strExpr.Length)
                {
                    m_currToken.Type = TokenType.T_EOL;
                    m_currToken.str = "EOL";
                    m_nNextPos = 0;
                    m_PreviousTokenType = TokenType.T_EOL;
                    return TokenType.T_EOL;
                }
                else if (isdigit(strExpr[m_nNextPos]))
                {
                    while (m_nNextPos < m_strExpr.Length && isdigit(strExpr[m_nNextPos]))
                    {
                        strToken.Append(strExpr[m_nNextPos]);
                        m_nNextPos++;
                    }

                    if (m_nNextPos < m_strExpr.Length && (strExpr[m_nNextPos] == '.' || strExpr[m_nNextPos] == ','))
                    {
                        strToken.Append(',');
                        m_nNextPos++;
                        while (m_nNextPos < m_strExpr.Length && isdigit(strExpr[m_nNextPos]))
                        {
                            strToken.Append(strExpr[m_nNextPos]);
                            m_nNextPos++;
                        }
                        m_PreviousTokenType = m_currToken.Type;
                        m_currToken.Type = TokenType.T_NUMBER;
                        m_currToken.str = strToken.ToString();
                        m_currToken.Value = Convert.ToDouble(strToken.ToString());
                        return TokenType.T_NUMBER;
                    }
                    else
                    {
                        m_PreviousTokenType = m_currToken.Type;
                        m_currToken.Type = TokenType.T_NUMBER;
                        m_currToken.str = strToken.ToString();
                        m_currToken.Value = Convert.ToDouble(strToken.ToString());
                        return TokenType.T_NUMBER;
                    }
                }
                else if (m_nNextPos < m_strExpr.Length && (strExpr[m_nNextPos] == '.' || strExpr[m_nNextPos] == ','))
                {
                    strToken.Append(',');
                    m_nNextPos++;
                    while (m_nNextPos < m_strExpr.Length && isdigit(strExpr[m_nNextPos]))
                    {
                        strToken.Append(strExpr[m_nNextPos]);
                        m_nNextPos++;
                    }
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_NUMBER;
                    m_currToken.str = strToken.ToString();
                    m_currToken.Value = Convert.ToDouble(strToken.ToString());
                    return TokenType.T_NUMBER;
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == '(')
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_OPAREN;
                    m_currToken.str = "(";
                    ++m_nNextPos;
                    return TokenType.T_OPAREN;
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == ')')
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_CPAREN;
                    m_currToken.str = ")";
                    ++m_nNextPos;
                    return TokenType.T_CPAREN;
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == '+')
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.str = "+";
                    ++m_nNextPos;
                    m_currToken.Type = TokenType.T_PLUS;
                    return TokenType.T_PLUS;
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == '-')
                {
                    m_currToken.str = "-";
                    ++m_nNextPos;
                    m_PreviousTokenType = m_currToken.Type;
                    if (m_PreviousTokenType == TokenType.T_CPAREN || m_PreviousTokenType == TokenType.T_NUMBER)
                    {
                        m_currToken.Type = TokenType.T_MINUS;
                        return TokenType.T_MINUS;
                    }
                    else
                    {
                        m_currToken.Type = TokenType.T_UMINUS;
                        return TokenType.T_UMINUS;
                    }
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == '*')
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_MULT;
                    m_currToken.str = "*";
                    ++m_nNextPos;
                    return TokenType.T_MULT;
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == '/')
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_DIV;
                    m_currToken.str = "/";
                    ++m_nNextPos;
                    return TokenType.T_DIV;
                }
                else if (m_nNextPos < m_strExpr.Length && strExpr[m_nNextPos] == '^')
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_EXP;
                    m_currToken.str = "^";
                    ++m_nNextPos;
                    return TokenType.T_EXP;
                }                
                else
                {
                    m_PreviousTokenType = m_currToken.Type;
                    m_currToken.Type = TokenType.T_UNKNOWN;
                    m_currToken.str = strExpr[m_nNextPos].ToString();
                    ++m_nNextPos;
                    return TokenType.T_UNKNOWN;
                }
            }
        }

        public Token m_currToken;
        private string m_strExpr;
        private int m_nNextPos;
        private TokenType m_PreviousTokenType;
    }
}

