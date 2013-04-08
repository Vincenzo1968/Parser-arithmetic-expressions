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


import java.io.Console;
//import java.util.Arrays;
//import java.io.IOException;

// javac CLexer.java CParser.java ExprJava.java
// java ExprJava

public class ExprJava
{
    public static void main(String[] args) /* throws IOException */
	{
		String str;
		CParser parser = new CParser();

		Console c = System.console();
		
		System.out.println();																
		
		while( true )
		{		
			//String strExpr = c.readLine("Inserisci un'espressione aritmetica: ");		
			String strExpr = c.readLine("Enter an arithmetic expression: ");		
			
			System.out.println();															
						
			if ( strExpr.length() == 0 )
				break;

			System.out.println();															

			if ( parser.Parse(strExpr) )
			{
				//System.out.print("Il risultato e': ");
				System.out.print("The result is: ");				
				System.out.println(parser.GetValue());				
				System.out.println();					
			}
		}
	
		/*
		CLexer lex = new CLexer();
		
		Console c = System.console();
					
		while( true )
		{
			System.out.println();									
		
			//String strExpr = c.readLine("Inserisci un'espressione aritmetica: ");
			String strExpr = c.readLine("Enter an arithmetic expression: ");				
			
			if ( strExpr.length() == 0 )
				break;

			System.out.println();													
			//System.out.println("Lista Token:");
			System.out.println("Token List:");
					
			lex.GetNextToken(strExpr);
			
			int x = 0;
				
			while ( lex.m_currToken.Type != CLexer.TokenTypeEnum.T_EOL)
			{
				System.out.println();													
							
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_PLUS )
					System.out.println("PLUS");					
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_MINUS )
					System.out.println("MINUS");										
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_MULT )
					System.out.println("MULT");									
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_DIV )
					System.out.println("DIV");									
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_UMINUS )
					System.out.println("UMINUS");									
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_CPAREN )
					System.out.println("CPAREN");									
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_OPAREN )
					System.out.println("OPAREN");									
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_NUMBER )
					System.out.println("NUMBER");									
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_UNKNOWN )
					System.out.println("UNKNOWN");									

				System.out.print("Token: <");
				System.out.print(lex.m_currToken.str);
				System.out.print(">");
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_NUMBER )
				{
					System.out.print(" Value: ");
					System.out.print(lex.m_currToken.Value);					
				}
				System.out.println();				
				
				lex.GetNextToken(strExpr);
				if ( lex.m_currToken.Type == CLexer.TokenTypeEnum.T_EOL )
					System.out.println("EOL");													
					
				System.out.println();													
					
					
				x++;
				if ( x >= 21 )
					break;
			}
			
			System.out.println();												
		}
		*/		
    }
}

