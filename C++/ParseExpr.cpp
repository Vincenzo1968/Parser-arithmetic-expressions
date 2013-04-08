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
#include <string>
using namespace std;

#include "Parser.h"
#include "Lexer.h"

int main()
{
	char strExpr[255];
	string str;
	CParser parser;

	while( true )
	{
		//cout << "Inserisci un'espressione aritmetica: ";
		cout << "Enter an arithmetic expression: ";		
		getline(cin, str);

		if ( strlen(str.c_str()) == 0 )
			break;

		strcpy(strExpr, str.c_str());

		if ( parser.Parse(strExpr) )
			//cout << endl << "Il risultato e': " << parser.GetValue() << endl << endl;
			cout << endl << "The result is: " << parser.GetValue() << endl << endl;			
	}

	return 0;
}

/*
int main()
{
	char strExpr[255];
	string str;
	CLexer lex;
	Token tok;

	while( true )
	{
		//cout << "Inserisci un'espressione aritmetica: ";
		cout << "Enter an arithmetic expression: ";		
		
		getline(cin, str);

		if ( strlen(str.c_str()) == 0 )
			break;

		strcpy(strExpr, str.c_str());

		//cout << "Lista Token:" << endl;
		cout << "Token List:" << endl;

		lex.GetNextToken(strExpr, &tok);
		while ( tok.Type != T_EOL )
		{
			if ( tok.Type == T_PLUS )
				cout << "PLUS" << endl;
			if ( tok.Type == T_MINUS )
				cout << "MINUS" << endl;
			if ( tok.Type == T_MULT )
				cout << "MULT" << endl;
			if ( tok.Type == T_DIV )
				cout << "DIV" << endl;
			if ( tok.Type == T_UMINUS )
				cout << "UMINUS" << endl;
			if ( tok.Type == T_CPAREN )
				cout << "CPAREN" << endl;
			if ( tok.Type == T_OPAREN )
				cout << "OPAREN" << endl;
			if ( tok.Type == T_NUMBER )
				cout << "NUMBER" << endl;
			if ( tok.Type == T_UNKNOWN )
				cout << "UNKNOWN" << endl;

			cout << "Token: <" << tok.str << ">" << endl;
			lex.GetNextToken(strExpr, &tok);
			if ( tok.Type == T_EOL )
				cout << "EOL" << endl;
		}
	} 
	
	return 0;
}
*/

