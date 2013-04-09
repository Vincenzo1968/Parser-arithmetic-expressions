/*
   Copyright (C) 2013 Vincenzo Lo Cicero

   Author: Vincenzo Lo Cicero.
   e-mail: vincenzo.locicero@live.it

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

using System;
//using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;

using ExprCS;

namespace ExprCS
{
    class Program
    {
        static void Main(string[] args)
        {
	        string strExpr;
            CParser parser = new CParser();

            while (true)
            {
                //Console.Write("Inserisci un'espressione aritmetica: ");
                Console.Write("Enter an arithmetic expression: ");                
                strExpr = Console.ReadLine();

                if (strExpr.Length == 0)
                    break;

                if (parser.Parse(strExpr))
                    //Console.WriteLine("Il risultato e': {0}", parser.GetValue());
                    Console.WriteLine("The result is: {0}", parser.GetValue());                    

                Console.WriteLine();
            }


            /*
	        string strExpr;
	        CLexer lex;
	        Token tok;

            lex = new CLexer();
	        while( true )
	        {
                //Console.Write("Inserisci un'espressione aritmetica: ");
                Console.Write("Enter an arithmetic expression: ");                
                
                strExpr = Console.ReadLine();

                if (strExpr.Length == 0)
                    break;

                lex.SetExpr(strExpr);

                Console.WriteLine();

                //Console.WriteLine("Lista Token:");
                Console.WriteLine("Token List:");

		        lex.GetNextToken();
                tok = lex.m_currToken;
                while ( tok.Type != TokenType.T_EOL )
	    {
                    if (tok.Type == TokenType.T_PLUS)
                        Console.WriteLine("PLUS");
                    if (tok.Type == TokenType.T_MINUS)
                        Console.WriteLine("MINUS");
                    if (tok.Type == TokenType.T_MULT)
                        Console.WriteLine("MULT");
                    if (tok.Type == TokenType.T_DIV)
                        Console.WriteLine("DIV");
                    if (tok.Type == TokenType.T_UMINUS)
                        Console.WriteLine("UMINUS");
                    if (tok.Type == TokenType.T_CPAREN)
                        Console.WriteLine("CPAREN");
                    if (tok.Type == TokenType.T_OPAREN)
				        Console.WriteLine("OPAREN");
                    if (tok.Type == TokenType.T_NUMBER)
                        Console.WriteLine("NUMBER");
                    if (tok.Type == TokenType.T_UNKNOWN)
				        Console.WriteLine("UNKNOWN");

                    if ( tok.Type == TokenType.T_NUMBER )
                        Console.WriteLine("Token: <{0}> Value: {1}", tok.str, tok.Value);
                    else
                        Console.WriteLine("Token: <{0}>", tok.str);
			        lex.GetNextToken();
                    tok = lex.m_currToken;
                    if (tok.Type == TokenType.T_EOL)
                        Console.WriteLine("EOL");

		        }
	        }
            */
        }
    }
}

