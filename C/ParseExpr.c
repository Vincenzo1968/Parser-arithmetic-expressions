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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* #include "lexer.h" */
#include "parser.h"
/* #include "parser_bnf.h" */

int main()
{
	char strExpr[255];
	double dblRes;
	
	while( 1 )
	{		
		printf("Enter an arithmetic expression: ");
		
		if ( fgets(strExpr, 254, stdin) == NULL )
			break;
			
		strExpr[strlen(strExpr) - 1] = '\0';
	
		if ( strlen(strExpr) == 0 )
			break;
		
		if ( Parse(strExpr, &dblRes) )
			printf("\nThe result is: %lf\n\n", dblRes);
	}

	return 0;
}

/*
int main()
{
	char strExpr[255];
	Token tok;
	
	while( 1 )
	{		
		printf("Enter an arithmetic expression: ");
		
		if ( fgets(strExpr, 254, stdin) == NULL )
			break;
			
		strExpr[strlen(strExpr) - 1] = '\0';

		if ( strlen(strExpr) == 0 )
			break;
				
		initToken(&tok);
	
		printf("\nToken List:\n\n");

		GetNextToken(strExpr, &tok);
		
		while ( tok.Type != T_EOL )
		{
			if ( tok.Type == T_PLUS )
				printf("PLUS\n");
			if ( tok.Type == T_MINUS )
				printf("MINUS\n");
			if ( tok.Type == T_MULT )
				printf("MULT\n");
			if ( tok.Type == T_DIV )
				printf("DIV\n");
			if ( tok.Type == T_UMINUS )
				printf("UMINUS\n");
			if ( tok.Type == T_UPLUS )
				printf("UPLUS\n");				
			if ( tok.Type == T_CPAREN )
				printf("CPAREN\n");
			if ( tok.Type == T_OPAREN )
				printf("OPAREN\n");
			if ( tok.Type == T_NUMBER )
				printf("NUMBER\n");
			if ( tok.Type == T_UNKNOWN )
				printf("UNKNOWN\n");

			printf("Token: <%s>\n", tok.str);
			GetNextToken(strExpr, &tok);	
					
			if ( tok.Type == T_EOL )
				printf("EOL\n");
		}
	} 
	
	return 0;
}
*/
