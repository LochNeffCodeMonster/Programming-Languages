#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() 
{
	char ch, file_name[25];
	File *fp;
	
	printf("Enter name of a file you wish to see\n");
	gets(file_name);
	
	fp = fopen(file_name, "r"); // read mode
	
	if (fp == NULL) 
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	
	printf("The contents of %s file are:\n", file_name);
	
	while((ch = fgetc(fp)) != EOF)
		printf("%c", ch);
		
	fclose(fp);
	return 0;
} 
	
		 
int currentTableIndex = 2;
int lineNumber = 1;
int position = 0;
int tokens[100];

char symbolTable[][2] = {
	{'DIV', 'OP'},
	{'MOD', 'OP'}
	}
	
	
int lookup(char strng1) {

	int i;
	length = ArraySize(symbolTable);

	for (i = 0; i < length; i++) 
	{
		if (strcmp(strng1, symbolTable[i]) == 0) 
		{
			return i;
		}
		
		else {
			return 0;
		}
	}
	

int insert(char c, char token) {
	
	insert into table
	
	return index;
}

int getToken(char **source, char *token) {
	
	enum {START, INT, FLOAT, ID, OP, ERROR, DONE} state = START;
	size_t i = 0;
	char *operators = "+-*/";
	char *underscore = "_";
	char *period = ".";

	if(**source == 0) 
		return EOF;
	
	while(**source!= 0) {
	
		switch(state) {
		
			case START:
				if(isalpha(**source)) {
					state = ID;
					token[i++] = *(*source)++; 
				}
				
				else if (isdigit(**source)) {
					state = INT;
					token[i++] = *(*source)++;
				}
				
				else if (isspace(**source)) {
					(*source)++; //ignore whitespace
				}
				break;
				
			case INT:
				// If character is a digit, append digit to token
				if(isdigit(**source)) {
					token[i++] = *(*source)++;
				}
				
				if(strchr(underscore, **source) != NULL) {
					state = FLOAT;
					token[i++] = *(*source)i++; //add period to the token
				}
				
				// If character is space, signal that we are done
				else if(isspace(**source)) {
					(*source)++;
					state = DONE;
				// We've read a non-digit character; terminate the token
				else {
					token[i++] = 0;
					state = ERROR;
				}
				break;
				
				
			case FLOAT:
				// If character is a digit, append digit to token
				if(isdigit(**source)) {
					token[i++] = *(*source)++;
				}
				// If character is space, signal that we are done
				else if(isspace(**source)) {
					(*source)++;
					state = DONE;
				// We've read a non-digit character; terminate the token
				else {
					token[i++] = 0;
					state = ERROR;
				}
				break;
				
			case ID:
				//if character is equal to a character
				if(isalpha(**source)) {
					token[i++] = *(*source)++;
				}
				
				else if(isdigit(**source)) {
					token[i++] = *(*source)++;
				}
				
				else if(strchr(underscore, **source) != NULL) {
					token[i++] = *(*source)i++;
				}
				
				// If character is space, signal that we are done
				else if(isspace(**source)) {
					(*source)++;
					state = DONE;
				}
				
				else {
					token[i++] = 0;
					state = ERROR;
				}
				break;
				
			case OP:
			
				if(isspace(**source)) {
					(*source)++;
					state = DONE;
				}
				
				else {
					token[i++] = 0;
					state = ERROR;
				}
				break;
				
			case DONE:
				return 1;
				break;
				
			case ERROR:
				return 0;
				break;		
		
		}
	
	}


}





void initTable() {

}
