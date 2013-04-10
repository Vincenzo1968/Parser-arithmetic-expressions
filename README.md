Parser-arithmetic-expressions
=============================

Arithmetic expressions recursive descent parser
-----------------------------------------------

The parser uses the following EBNF grammar:

    expr : expr1 {('+' | '-') expr1};
    expr1 : expr2 {('*' | '/') expr2};
    expr2 : ['-'] expr3;
    expr3 : expr4 {'^' expr4}
    expr4 : T_NUMBER | '(' expr ')'    

The non-terminal symbols of the grammar are:

    expr expr1 expr2 expr3 expr4

*expr* is the initial symbol.

The terminal symbols are:

    '+', '-', '*', '/', '^', '(', ')', 'T_NUMBER'

The braces are used to indicate zero or more occurrences.
The square brackets are used to indicate zero or one occurrence.
Parentheses are used to indicate the groupings.
The vertical bar, *|*, indicates a choice between two options.

For example, the first rule of the grammar

    expr: expr1 {('+' | '-') expr1};

tells us that:
expr is an expression consisting of expr1 followed by zero or more
repetitions of the symbol '+' or '-' symbol followed by expr1.

The algorithm is to implement a function for each non-terminal symbol.
For non-terminal symbols is called the function.
For terminal symbols you check the matching symbol
to that provided by the grammatical rule and, if successful,
reads the next token from the input string. If, instead, the
symbol does not match, it returns an error message.

An example, in C-like pseudocode, related to the first grammar rule:

```c
    /* expr  : expr1 {('+' | '-') expr1}; */
    int expr()
    {
        if ( !expr1() )
            return 0;

        while ( Token == T_PLUS || Token == T_MINUS )
        {
            GetNextToken();

            if ( !expr1() )
                return 0;
        }
        
        return 1;
    }
```



Il parser usa la seguente grammatica EBNF:
   
    expr : expr1 {('+' | '-') expr1};
    expr1 : expr2 {('*' | '/') expr2};
    expr2 : ['-'] expr3;
    expr3 : expr4 {'^' expr4}
    expr4 : T_NUMBER | '(' expr ')'    
    

	
I simboli non terminali della grammatica sono:

    expr expr1 expr2 expr3 expr4

*expr* è il simbolo iniziale.

I simboli terminali sono:

    '+', '-', '*', '/', '^', '(', ')', 'T_NUMBER'


Le parentesi graffe sono usate per indicare zero o più occorrenze.
Le parentesi quadre sono usate per indicare zero o una occorrenza.
Le parentesi tonde sono usate per indicare i raggruppamenti.
La barra verticale, *|*, indica una scelta tra due opzioni.

Per esempio, la prima regola della grammatica

    expr  : expr1 {('+' | '-') expr1};

ci dice che: 
expr è un'espressione formata da expr1 seguita da: zero o più
ripetizioni del simbolo '+' o del simbolo '-' seguito da expr1.

L'algoritmo a discesa ricorsiva consiste nell'implementare una
funzione per ogni simbolo non terminale.
Per i simboli non terminali viene richiamata la relativa funzione.
Per i simboli terminali si controlla che il simbolo corrisponda
a quello previsto dalla regola grammaticale e, in caso di successo,
si legge il prossimo token dalla stringa di input. Se, invece, il 
simbolo non corrisponde, si riporta un messaggio d'errore.

Un esempio, in pseudocode simil-C, relativo alla prima regola grammaticale:

```c
    /* expr  : expr1 {('+' | '-') expr1}; */
    int expr()
    {
        if ( !expr1() )
            return 0;

        while ( Token == T_PLUS || Token == T_MINUS )
        {
            GetNextToken();

            if ( !expr1() )
                return 0;
        }
        
        return 1;
    }
```
