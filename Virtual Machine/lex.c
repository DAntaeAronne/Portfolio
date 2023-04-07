/* Authors: Ethan Ellis & D'Antae Aronne
   Course: COP3402-22Spring 0002
   Assignment: HW2 - Lexical Analyzer
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;
char reserved[13][30] = {"const", "var", "procedure", "call", "if", "then", "else", "while", "do", "begin", "end", "read", "write"};
char symbols[16][5] = {"==", "!=", "<", "<=", ">", ">=", "*", "/", "+", "-", "(", ")", ",", ".", ";", ":="};

void printlexerror(int type);
void printtokens();
void tokenizeType(char* sym);


lexeme *lexanalyzer(char *input, int printFlag)
{
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	lex_index = 0;
  int i = 0;
  int j;
  int ignoreCom = 0;
  int ignoreTab = 0;
  int MeCom = 0;
 

  // Adding tokens to list
  while(input[i] != '\0'){
    // String of the current word/character and the index 
    char *Current = malloc(sizeof(char) * MAX_NUMBER_TOKENS);
    int CurIn = 0;
    int digits = 0;
    int TokenFound = 0;    
    
    // While the current character is not a whitespace, comma, or semicolon
    // add the the character to a string which is the current word
    while(!iscntrl(input[i]) && !isspace(input[i]) && (input[i] != ',') && (input[i] != ';') && (input[i] != '\t') && (input[i] != '.')){
      Current[CurIn] = input[i];
      CurIn++;
      i++;
    } // End of While
      
    // If Current is the start of a comment
    // Then flag ignore
    if((strcmp(Current,"/*") == 0)){
     ignoreCom= 1;
    }
    
    if(iscntrl(input[i]) && ((input[i - 1] == '\t') || isspace(input[i-1]) || iscntrl(input[i-1]) || (input[i-1] == '.'))){
      ignoreTab = 1;
    }
    
    // Check if Current is a comment with no space after "/*"
    if(Current[0] == '/' && Current[1] == '*'){
      ignoreCom = 1;
    } // End of If

    // If input is a tab
    // Then flag Tabignore
    if(input[i] == '\t'){
      ignoreTab = 1;
    }
    
    // If ignoreCom is flagged
    // Then don't search for tokens
  
    if(!ignoreCom && !ignoreTab){

      // When a whitespace is reached
      // check to which type the token is
      
      // Checking for if it is a number
      if(isdigit(Current[0])){
        for(CurIn = 0; CurIn < strlen(Current); CurIn++){
          if(isdigit(Current[CurIn])){
            digits++;

            // Displays error message for exceeded digit length
            if(digits > MAX_NUMBER_LEN){
              printlexerror(2);
              return NULL;
            } // End of If
          } // End of If

          // Displays error message for Invalid Identifier
          else{
            printlexerror(1);
            return NULL;
          } // End of Else
        } // End of For
      } // End of If
      // If the whole string is a number
      // Then set the token type to numbersym, set the value at the current index of list, and flag TokenFound
      if(digits == strlen(Current)){
        tokenizeType("num");
        list[lex_index].value = atoi(Current);
        TokenFound = 1;
        lex_index++;
      } // End of If
  
      // If the token hasn't been found
      // Then check to see if the token is a reserved word    
      if(!TokenFound){
        for(j = 0; j < 13; j++){
          // If the token is a reserved word then set the type and flag TokenFound
          if(strcmp(Current, reserved[j]) == 0){               
          
            tokenizeType(reserved[j]);
            TokenFound = 1;
            lex_index++;
          } // End of If
        } // End of For
      } // End of If
      
      // If the token hasn't been found
      // Then check to see if the token is a symbol    
      if(!TokenFound){
        for(j = 0; j < 16; j++){
          // If the token is a symbol then set the type and flag TokenFound
          if(strcmp(Current, symbols[j]) == 0){     
            tokenizeType(symbols[j]);
            TokenFound = 1;
            lex_index++;
          } // End of If
        } // End of For
      } // End of If
      
      // If the token still hasn't been found
      // Then the token must be an identifier and will set it as such
      if(!TokenFound){
        // Display an error message for exceeding identifier length.
        if(strlen(Current) > MAX_IDENT_LEN){
          printlexerror(3);
          return NULL;
        } // End of If

        // Display an error message for invalid symbols.
        for(int j = 0; j < strlen(Current); j++){
          if(isalpha(Current[j]) == 0 && isdigit(Current[j]) == 0){
            printlexerror(4);
            return NULL;
          } // End of If
        } // End of For
        
        tokenizeType("iden");
        strcpy(list[lex_index].name,Current);
        lex_index++;
      } // End of If
      
   } // End of "Ignore" If
   
   
    // If Curent is the end of a comment and ignoreCom is flagged
    // Then deflag ignore
    if(strcmp(Current,"*/") == 0 && (ignoreCom == 1)){
     ignoreCom= 0;
    }
    if((Current[CurIn - 1] == '/') && (Current[CurIn - 2] == '*')){
      ignoreCom= 0;
    }
   
   // If the input ended on a comma
   // Then add it to the list
   if(input[i] == ','){
     tokenizeType(",");
     lex_index++;
     i++;
   }
   
   // If the input ended on a semicolon
   // Then add it to the list
   if(input[i] == ';'){
     tokenizeType(";");
     lex_index++;
     i++;
   }
   
   if(input[i] == '.'){
     tokenizeType(".");
     lex_index++;
   }
   
   // Increment i to the next character
   i++;

   
   // If the input is not a tab and Tabignore is flagged
   // Then set Tabignore to 0
   if((input[i] != '\t') && (ignoreTab == 1)){
      ignoreTab = 0;
    }
   
  } // End of While
 
  // Display an error message for a never-ending comment.
  if (ignoreCom == 1){
    printlexerror(5);
    return NULL;
  } // End of If

  
	if (printFlag)
		printtokens();
  
  
 
	// these last two lines are really important for the rest of the package to run
	list[lex_index].type = -1;
 
	return list;
}


void tokenizeType(char* sym){

  if (strcmp(sym,"==") == 0){
    list[lex_index].type = eqlsym;
	}
	else if (strcmp(sym, "!=") == 0){
		list[lex_index].type = neqsym;
	}
	else if (strcmp(sym, "<") == 0){
		list[lex_index].type = lsssym;
	}
	else if (strcmp(sym, "<=") == 0){
  	list[lex_index].type = leqsym;
	}
	else if (strcmp(sym,">") == 0){
		list[lex_index].type = gtrsym;
	}
	else if (strcmp(sym,">=") == 0){
		list[lex_index].type = geqsym;
	}
	else if (strcmp(sym,"*") == 0){
		list[lex_index].type = multsym;
	}
	else if (strcmp(sym,"/") == 0){
    list[lex_index].type = divsym;
	}
	else if (strcmp(sym,"+") == 0){
    list[lex_index].type = plussym;
	}
	else if (strcmp(sym,"-") == 0){
    list[lex_index].type = minussym;
	}
	else if (strcmp(sym,"(") == 0){
	  list[lex_index].type = lparensym;
	}
	else if (strcmp(sym,")") == 0){
	  list[lex_index].type = rparensym;
	}
	else if (strcmp(sym,",") == 0){
    list[lex_index].type = commasym;
	}
	else if (strcmp(sym,".") == 0){
    list[lex_index].type = periodsym;
	}
	else if (strcmp(sym,";") == 0){
    list[lex_index].type = semicolonsym;
	}
	else if (strcmp(sym,":=") == 0){
    list[lex_index].type = assignsym;
	}
	else if (strcmp(sym,"begin") == 0){
    list[lex_index].type = beginsym;
	}
	else if (strcmp(sym,"end") == 0){
    list[lex_index].type = endsym;
	}
	else if (strcmp(sym,"if") == 0){
    list[lex_index].type = ifsym;
	}
	else if (strcmp(sym,"then") == 0){
    list[lex_index].type = thensym;
	}
	else if (strcmp(sym,"else") == 0){
    list[lex_index].type = elsesym;
	}
	else if (strcmp(sym,"while") == 0){
    list[lex_index].type = whilesym;
	}
	else if (strcmp(sym,"do") == 0){
    list[lex_index].type = dosym;
	}
	else if (strcmp(sym,"call") == 0){
    list[lex_index].type = callsym;
	}
	else if (strcmp(sym,"write") == 0){
    list[lex_index].type = writesym;
	}
	else if (strcmp(sym,"read") == 0){
    list[lex_index].type = readsym;
	}
	else if (strcmp(sym,"const") == 0){
    list[lex_index].type = constsym;
	}
	else if (strcmp(sym,"var") == 0){
    list[lex_index].type = varsym;
	}
	else if (strcmp(sym,"procedure") == 0){
    list[lex_index].type = procsym;
	}
	else if (strcmp(sym,"iden") == 0){
    list[lex_index].type = identsym;
	}
	else if (strcmp(sym,"num") == 0){
	  list[lex_index].type = numbersym;
	}

}


void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
 
	for (i = 0; i < lex_index; i++)
	{
 
		switch (list[i].type)
		{
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case plussym:
				printf("%11s\t%d", "+", plussym);
				break;
			case minussym:
				printf("%11s\t%d", "-", minussym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
}


void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Number Length\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Identifier Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 5)
		printf("Lexical Analyzer Error: Never-ending comment\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}