/*
 * parser.c
 *
 *  Created on: Apr 30, 2019
 *      Author: JonathanNeff
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "lexan.h"

#ifndef LEXAN

// This is meant to prevent this file from being included twice.
#define LEXAN

// Table sizes
#define SYMBOL_TABLE_CAPACITY 1000
#define MAX_TOKENS 10000



// *******************************************************************
// Data types
enum token_type {NONE=101, OP, ID, INT, FLOAT};


struct symEntry {
    char name[BUFSIZ];
    enum token_type tokenType;
};


struct token {
    enum token_type tokenType;
    int intValue;               // If tokenType == INT
    float floatValue;           // If tokenType == FLOAT

    // MOD and DIV are operators of size 4 (including the '\0').
    // We could potentially have longer operators, but that is unlikely.
    char operator[10];          // If tokenType == OP

    // If the tokenType is an ID, the symbolEntry will hold the
    // index in the SymbolTable.
    int symbolEntry;            // If tokenType == ID

    int lineno;                 // Which source line is this token from.
                                // This is useful for printing error
                                // messages.
};
// *******************************************************************


// *******************************************************************
// Function declarations
int lexan ();
void initSymbolTable();
int lookup(char *s);
int insert(char *s, enum token_type tokenType);
void printTable();
void emit(enum token_type tokenType, char *s, int tokenValInt,
                          float tokenValFloat, int symbolTableId, int lineno);
void error(int c);
// *******************************************************************


// *******************************************************************
// Global Data
// These are declared as extern, which means that we are only declaring
// their existance, not instantiating them.  They are created in the lexan.c
// file. The reason this is necessary is so that we can have declarations of
// this data to be used in other files, like parser.c, but we don't
// want multiple instantiations.

extern int lineno;
extern int symTableSize;   // Last entry in the symbol table
extern int tokenTableSize;   // Last entry in the token table

// Symbol Table
extern struct symEntry SymbolTable[SYMBOL_TABLE_CAPACITY];

// Token Table
extern struct token ListOfTokens[MAX_TOKENS];

// *******************************************************************

#endif

//-----------------------------------------------------------------------------------
// Global Data

int lookahead; // Holds the index of the token following the next token

struct token nextToken; // The token currently being processed

// Predefined tokens to be used when calling match()
// DIV and MOD are in the SymbolTable as entries
// 0 and 1.

struct token MULT_TOK = {OP, 0, 0.0, "*", -1, 0};
struct token DIVI_TOK = {OP, 0, 0.0, "/", -1, 0};
struct token ADDI_TOK = {OP, 0, 0.0, "+", -1, 0};
struct token SUBT_TOK = {OP, 0, 0.0, "-", -1, 0};
struct token DIV_TOK = {OP, 0, 0.0, "DIV", 0, 0};
struct token MOD_TOK = {OP, 0, 0.0, "MOD", 1, 0};
struct token LPAR_TOK = {OP, 0, 0.0, "(", -1, 0};
struct token RPAR_TOK = {OP, 0, 0.0, ")", -1, 0};
struct token INT_TOK = {INT, 0, 0.0, "", -1, 0};
struct token FLOAT_TOK = {FLOAT, 0, 0.0, "", -1, 0};
struct token ID_TOK = {ID, 0, 0.0, "", -1, 0};

//--------------------------------------------------------------------------------------
// Function declarations
int main(int argc, char *argv[]);

void parse()
{
	struct token firstToken = ListOfTokens[lookahead];
	lookahead ++;
	while (lookahead <= tokenTableSize) {
		expr();
	}
}

void syntaxError(char *message, struct token t) {
	// The error should include the message and the line number (which can be retrieved from the token).
	fprintf(stderr, "expected token: %d(%c), got: %d(%c)\n", t, t, nextToken, nextToken);
}


void match(struct token token) {
	if (token.tokenType == nextToken.tokenType)
	{
		if (token.tokenType == OP)
		{
			if (token.operator == nextToken.operator)
			{
				next();	// Get the next token
			}
			else
			{
				syntaxError(token);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			next();	// Get the next token
		}
	}
	else
	{
		syntaxError(token);
		exit(EXIT_FAILURE);
	}
}


// The factor function is used to process the <factor> non-terminal.
void factor() {

	if (nextToken.operator == '(')
	{
		match(LPAR_TOK);		// Call the match() function with the left parenthesis token
		expr();  			// Call the expr() function (according to the new grammar)
		match(RPAR_TOK);		// Call the match() function with the right parenthesis token
	}
	// If the next token is an INT, then print the appropriate value and match the token
	if (nextToken.tokenType == INT) {
		tokenToString(INT_TOK);
		match(INT_TOK);
	}
	// If the next token is an FLOAT, then print the appropriate value and match the token
	if (nextToken.tokenType == FLOAT) {
		tokenToString(FLOAT_TOK);
		match(FLOAT_TOK);
	}
	// If the next token is an symbol then print the appropriate value and match the token
	if (nextToken.tokenType == ID) {
		tokenToString(ID_TOK);
		match(ID_TOK);
	}
	else {
		syntaxError();
		exit(EXIT_FAILURE);
	}
}

// The term function is used to process the <term> non-terminal.
void term() {
	factor();
	term_new();
}

void term_new()
{
	if (nextToken.operator == '*') {
		match(MULT_TOK);
		factor();
		tokenToString(MULT_TOK);
	}
	else if (nextToken.operator == '/') {
		match(DIVI_TOK);
		factor();
		tokenToString(DIVI_TOK);
	}
	else if (nextToken.operator == 'MOD') {
		match(MOD_TOK);
		factor();
		tokenToString(MOD_TOK);
	}
	else if (nextToken.operator == 'DIV') {
		match(DIV_TOK);
		factor();
		tokenToString(DIV_TOK);
	}
	else {
		return;
	}
}

// The expr function is used to process the <expr> non-terminal in the grammar.
void expr()
{
	term();
	expr_new();
}

void expr_new()
{
	if (nextToken.operator == '+') {
        match(ADDI_TOK);
        term();
        tokenToString(ADDI_TOK);
    }
    else if (nextToken.operator == '-') {
        match(SUBT_TOK);
        term();
        tokenToString(SUBT_TOK);
    }
    else {
        return;
    }
}

void next() {
	nextToken = ListOfTokens[lookahead];
	lookahead++;
}


char *tokenToString(struct token nextToken)
{
	static char result[BUFSIZ];  // Static so the string still exists after the function returns
	switch (nextToken.tokenType) {
	// We need to use snprintf() to avoid a buffer overflow.
	case OP:
		snprintf(result, BUFSIZ-1, "OP %s ", nextToken.operator);
		break;
	case ID:
		snprintf(result, BUFSIZ-1, "ID %s ", SymbolTable[nextToken.symbolEntry].name);
		break;
	case INT:
		snprintf(result, BUFSIZ-1. "INT %d ", nextToken.intValue);
		break;
	case FLOAT:
		snprintf(result, BUFSIZ-1, "FLOAT %f ", nextToken.floatValue);
		break;
	default:
		snprintf(result, BUFSIZ-1, "UNKNOWN ");
		break;
	}
	return result;
}

void initSymbolTable() {

	struct symEntry symEnt1 = {5, INT};
	SymbolTable[symTableSize] = symEnt1;
	symTableSize++;
	struct symEntry symEnt2 = {'*', OP};
	SymbolTable[symTableSize] = symEnt2;
	symTableSize++;
	struct symEntry symEnt3 = {'(', OP};
	SymbolTable[symTableSize] = symEnt3;
	symTableSize++;
	struct symEntry symEnt4 = {6, INT};
	SymbolTable[symTableSize] = symEnt4;
	symTableSize++;
	struct symEntry symEnt5 = {'y', ID};
	SymbolTable[symTableSize] = symEnt5;
	symTableSize++;
	struct symEntry symEnt6 = {'-', OP};
	SymbolTable[symTableSize] = symEnt6;
	symTableSize++;
	struct symEntry symEnt7 = {1, INT};
	SymbolTable[symTableSize] = symEnt7;
	symTableSize++;
	struct symEntry symEnt8 = {')', OP};
	SymbolTable[symTableSize] = symEnt8;
	symTableSize++;
}

void initTokenTable()
{
	struct token temp1 = {INT, 5, 0.0, "", 2, 0};
	ListOfTokens[tokenTableSize] = temp1;
	tokenTableSize++;
	struct token temp2 = {OP, 0, 0.0, "*", 3, 0};
	ListOfTokens[tokenTableSize] = temp2;
	tokenTableSize++;
	struct token temp3 = {OP, 0, 0.0, "(", 4, 0};
	ListOfTokens[tokenTableSize] = temp3;
	tokenTableSize++;
	struct token temp4 = {INT, 6, 0.0, "", 5, 0};
	ListOfTokens[tokenTableSize] = temp4;
	tokenTableSize++;
	struct token temp5 = {OP, 0, 0.0, "*", 3, 0};
	ListOfTokens[tokenTableSize] = temp5;
	tokenTableSize++;
	struct token temp6 = {ID, 0, 0.0, "", 6, 0};
	ListOfTokens[tokenTableSize] = temp6;
	tokenTableSize++;
	struct token temp7 = {OP, 0, 0.0, "-", 7, 0};
	ListOfTokens[tokenTableSize] = temp7;
	tokenTableSize++;
	struct token temp8 = {INT, 1, 0.0, "", 8, 0};
	ListOfTokens[tokenTableSize] = temp8;
	tokenTableSize++;
	struct token temp9 = {OP, 0, 0.0, ")", 9, 0};
	ListOfTokens[tokenTableSize] = temp9;
	tokenTableSize++;
}

int main ()
{
	symTableSize = 0;
	tokenTableSize = 0;
	initSymbolTable();
	initTokenTable();
	lookahead = 0;
	parse();
}





/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

/*
 * parser.c
 *
 *  Created on: Apr 30, 2019
 *      Author: JonathanNeff
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "lexan.h"

#ifndef LEXAN

// This is meant to prevent this file from being included twice.
#define LEXAN

// Table sizes
#define SYMBOL_TABLE_CAPACITY 1000
#define MAX_TOKENS 10000



// *******************************************************************
// Data types
enum token_type {NONE=101, OP, ID, INT, FLOAT};


struct symEntry {
    char name[BUFSIZ];
    enum token_type tokenType;
};


struct token {
    enum token_type tokenType;
    int intValue;               // If tokenType == INT
    float floatValue;           // If tokenType == FLOAT

    // MOD and DIV are operators of size 4 (including the '\0').
    // We could potentially have longer operators, but that is unlikely.
    char operator[10];          // If tokenType == OP

    // If the tokenType is an ID, the symbolEntry will hold the
    // index in the SymbolTable.
    int symbolEntry;            // If tokenType == ID

    int lineno;                 // Which source line is this token from.
                                // This is useful for printing error
                                // messages.
};
// *******************************************************************


// *******************************************************************
// Function declarations
int lexan ();
void initSymbolTable();
int lookup(char *s);
int insert(char *s, enum token_type tokenType);
void printTable();
void emit(enum token_type tokenType, char *s, int tokenValInt,
                          float tokenValFloat, int symbolTableId, int lineno);
void error(int c);

// Function declarations
//int main(int argc, char *argv[]);
int main();
void parse();
//void syntaxError (char *message, struct token t);
void syntaxError (struct token t);
void match(struct token t);
void factor();
void term();
void expr();
char *tokenToString(struct token nextToken);
void next();
void term_new();
void expr_new();
// *******************************************************************


// *******************************************************************
// Global Data
// These are declared as extern, which means that we are only declaring
// their existance, not instantiating them.  They are created in the lexan.c
// file. The reason this is necessary is so that we can have declarations of
// this data to be used in other files, like parser.c, but we don't
// want multiple instantiations.

extern int lineno;
extern int symTableSize;   // Last entry in the symbol table
extern int tokenTableSize;   // Last entry in the token table

// Symbol Table
extern struct symEntry SymbolTable[SYMBOL_TABLE_CAPACITY];

// Token Table
extern struct token ListOfTokens[MAX_TOKENS];

// *******************************************************************

#endif

//-----------------------------------------------------------------------------------
// Global Data

int lookahead; // Holds the index of the token following the next token
int lineno = 1;
int symTableSize = 0;
int tokenTableSize = 0;
struct token nextToken; // The token currently being processed
struct symEntry SymbolTable[SYMBOL_TABLE_CAPACITY];
struct token ListOfTokens[MAX_TOKENS];

// Predefined tokens to be used when calling match()
// DIV and MOD are in the SymbolTable as entries
// 0 and 1.

struct token MULT_TOK = {OP, 0, 0.0, "*", -1, 0};
struct token DIVI_TOK = {OP, 0, 0.0, "/", -1, 0};
struct token ADDI_TOK = {OP, 0, 0.0, "+", -1, 0};
struct token SUBT_TOK = {OP, 0, 0.0, "-", -1, 0};
struct token DIV_TOK = {OP, 0, 0.0, "DIV", 0, 0};
struct token MOD_TOK = {OP, 0, 0.0, "MOD", 1, 0};
struct token LPAR_TOK = {OP, 0, 0.0, "(", -1, 0};
struct token RPAR_TOK = {OP, 0, 0.0, ")", -1, 0};
struct token INT_TOK = {INT, 0, 0.0, "", -1, 0};
struct token FLOAT_TOK = {FLOAT, 0, 0.0, "", -1, 0};
struct token ID_TOK = {ID, 0, 0.0, "", -1, 0};

//--------------------------------------------------------------------------------------
// Function declarations


void parse()
{
	struct token firstToken = ListOfTokens[lookahead];
	lookahead ++;
	while (lookahead <= tokenTableSize) {
		expr();
	}
}
// char *message,
void syntaxError(struct token t) {
	// The error should include the message and the line number (which can be retrieved from the token).
	fprintf(stderr, "expected token: %d(%c), got: %d(%c)\n", t, t, nextToken, nextToken);
}


void match(struct token token) {
	if (token.tokenType == nextToken.tokenType)
	{
		if (token.tokenType == OP)
		{
			if (token.operator[0] == nextToken.operator[0])
			{
				next();	// Get the next token
			}
			else
			{
				syntaxError(token);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			next();	// Get the next token
		}
	}
	else
	{
		syntaxError(token);
		exit(EXIT_FAILURE);
	}
}


// The factor function is used to process the <factor> non-terminal.
void factor() {

	if (nextToken.operator[0] == '(')
	{
		match(LPAR_TOK);		// Call the match() function with the left parenthesis token
		expr();  			// Call the expr() function (according to the new grammar)
		match(RPAR_TOK);		// Call the match() function with the right parenthesis token
	}
	// If the next token is an INT, then print the appropriate value and match the token
	if (nextToken.tokenType == INT) {
		tokenToString(INT_TOK);
		match(INT_TOK);
	}
	// If the next token is an FLOAT, then print the appropriate value and match the token
	if (nextToken.tokenType == FLOAT) {
		tokenToString(FLOAT_TOK);
		match(FLOAT_TOK);
	}
	// If the next token is an symbol then print the appropriate value and match the token
	if (nextToken.tokenType == ID) {
		tokenToString(ID_TOK);
		match(ID_TOK);
	}
	else {
		syntaxError(nextToken);
		exit(EXIT_FAILURE);
	}
}

// The term function is used to process the <term> non-terminal.
void term() {
	factor();
	term_new();
}

void term_new()
{
	if (nextToken.operator[0] == '*') {
		match(MULT_TOK);
		factor();
		tokenToString(MULT_TOK);
	}
	else if (nextToken.operator[0] == '/') {
		match(DIVI_TOK);
		factor();
		tokenToString(DIVI_TOK);
	}
	else if (nextToken.operator[0] == 'M') {
		match(MOD_TOK);
		factor();
		tokenToString(MOD_TOK);
	}
	else if (nextToken.operator[0] == 'D') {
		match(DIV_TOK);
		factor();
		tokenToString(DIV_TOK);
	}
	else {
		return;
	}
}

// The expr function is used to process the <expr> non-terminal in the grammar.
void expr()
{
	term();
	expr_new();
}

void expr_new()
{
	if (nextToken.operator[0] == '+') {
        match(ADDI_TOK);
        term();
        tokenToString(ADDI_TOK);
    }
    else if (nextToken.operator[0] == '-') {
        match(SUBT_TOK);
        term();
        tokenToString(SUBT_TOK);
    }
    else {
        return;
    }
}

void next() {
	nextToken = ListOfTokens[lookahead];
	lookahead++;
}


char *tokenToString(struct token nextToken)
{
	static char result[BUFSIZ];  // Static so the string still exists after the function returns
	switch (nextToken.tokenType) {
	// We need to use snprintf() to avoid a buffer overflow.
	case OP:
		snprintf(result, BUFSIZ-1, "OP %s ", nextToken.operator);
		break;
	case ID:
		snprintf(result, BUFSIZ-1, "ID %s ", SymbolTable[nextToken.symbolEntry].name);
		break;
	case INT:
		snprintf(result, BUFSIZ-1, "INT %d ", nextToken.intValue);
		break;
	case FLOAT:
		snprintf(result, BUFSIZ-1, "FLOAT %f ", nextToken.floatValue);
		break;
	default:
		snprintf(result, BUFSIZ-1, "UNKNOWN ");
		break;
	}
	return result;
}

void initSymbolTable() {

	struct symEntry symEnt1 = {5, INT};
	SymbolTable[symTableSize] = symEnt1;
	symTableSize++;
	struct symEntry symEnt2 = {'*', OP};
	SymbolTable[symTableSize] = symEnt2;
	symTableSize++;
	struct symEntry symEnt3 = {'(', OP};
	SymbolTable[symTableSize] = symEnt3;
	symTableSize++;
	struct symEntry symEnt4 = {6, INT};
	SymbolTable[symTableSize] = symEnt4;
	symTableSize++;
	struct symEntry symEnt5 = {'y', ID};
	SymbolTable[symTableSize] = symEnt5;
	symTableSize++;
	struct symEntry symEnt6 = {'-', OP};
	SymbolTable[symTableSize] = symEnt6;
	symTableSize++;
	struct symEntry symEnt7 = {1, INT};
	SymbolTable[symTableSize] = symEnt7;
	symTableSize++;
	struct symEntry symEnt8 = {')', OP};
	SymbolTable[symTableSize] = symEnt8;
	symTableSize++;
}

void initTokenTable()
{
	struct token temp1 = {INT, 5, 0.0, "", 2, 0};
	ListOfTokens[tokenTableSize] = temp1;
	tokenTableSize++;
	struct token temp2 = {OP, 0, 0.0, "*", 3, 0};
	ListOfTokens[tokenTableSize] = temp2;
	tokenTableSize++;
	struct token temp3 = {OP, 0, 0.0, "(", 4, 0};
	ListOfTokens[tokenTableSize] = temp3;
	tokenTableSize++;
	struct token temp4 = {INT, 6, 0.0, "", 5, 0};
	ListOfTokens[tokenTableSize] = temp4;
	tokenTableSize++;
	struct token temp5 = {OP, 0, 0.0, "*", 3, 0};
	ListOfTokens[tokenTableSize] = temp5;
	tokenTableSize++;
	struct token temp6 = {ID, 0, 0.0, "", 6, 0};
	ListOfTokens[tokenTableSize] = temp6;
	tokenTableSize++;
	struct token temp7 = {OP, 0, 0.0, "-", 7, 0};
	ListOfTokens[tokenTableSize] = temp7;
	tokenTableSize++;
	struct token temp8 = {INT, 1, 0.0, "", 8, 0};
	ListOfTokens[tokenTableSize] = temp8;
	tokenTableSize++;
	struct token temp9 = {OP, 0, 0.0, ")", 9, 0};
	ListOfTokens[tokenTableSize] = temp9;
	tokenTableSize++;
}

int main ()
{
	symTableSize = 0;
	tokenTableSize = 0;
	initSymbolTable();
	initTokenTable();
	lookahead = 0;
	parse();
}
