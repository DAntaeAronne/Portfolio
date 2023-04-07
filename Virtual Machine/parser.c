/* Authors: Ethan Ellis & D'Antae Aronne
   Course: COP3402-22Spring 0002
   Assignment: HW1 - VM
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 150
#define MAX_SYMBOL_COUNT 50
#define MAX_REG_HEIGHT 10

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int level;
int RegSpace;
int lIndex;
int errflg;
int maddr;
int mIdx;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void mark();
int multipledeclarationcheck(char name[]);
int findsymbol(char name[], int kind);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
void Program(lexeme *list);
void Block(lexeme *list);
void ConstDeclar(lexeme *list);
int VarDeclar(lexeme *list);
void ProcDeclar(lexeme *list);
void Statement(lexeme *list);
void Condition(lexeme *list);
void Expression(lexeme *list);
void Term(lexeme *list);
void Factor(lexeme *list);


instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
	table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);
	tIndex = 0;
	cIndex = 0;
  lIndex = 0;
  RegSpace = 0;
  errflg = 0;
  maddr = 0;  
   
  Program(list);
  if(errflg){
    return NULL;
  }
  
	if (printTable){
		printsymboltable();
  }
	if (printCode){
		printassemblycode();
  }
   
	code[cIndex].opcode = -1;
	return code;
}


// Main code for parser to fulfil its function
void Program(lexeme *list){

  // JMP
  emit(7,0,0);
  
  addToSymbolTable(3,"main",0,0,0,0);
  level = -1;
  
  Block(list);  
  if(errflg){
    return;
  } // end of errflg if
  
  // If the current token isn't a period
  // Then call error 1 and stop  
  
  if(list[lIndex].type != periodsym){
    printparseerror(1);
     errflg = 1;
     return;
  } // end of error 1 If
  
  // HALT
  emit(9,0,3);
  
  // Fix address of main
  table[0].addr = maddr;
  code[0].m = maddr;
  
  // Fix address of CALs
  int TI;
  for(int j = 0; j < cIndex; j++){
      if(code[j].opcode == 5){
        TI = code[j].m;
        code[j].m = table[TI].addr;
      }
  } // end of For
  return;
} // end of Program


void Block(lexeme *list){
  level++;
  int x = 0;
  
  int pIndex = tIndex - 1;  
    
  // If there are any consts
  // Then check to see if symbol already in list and add them if not
  if(list[lIndex].type == constsym){
    ConstDeclar(list);
    if(errflg){
      return;
    } // end of errflg if
  } // end of Const If
  
  // If there are any vars
  // Then check to see if symbol already in list and add them if not
  if(list[lIndex].type == varsym){
    x = VarDeclar(list);
    if(errflg){
      return;
    } // end of errflg if
  } // end of Var If
  
  // If there are any procedures
  // Then check to see if symbol already in list and add them if not
  while(list[lIndex].type == procsym){
    ProcDeclar(list);
    if(errflg){
      return;
    } // end of errflg if
  } // end of Proc If
  
  table[pIndex].addr = cIndex; 
  
  // INC
  emit(6, 0, x + 3);
  
  maddr = cIndex - 1;
  
  Statement(list);
  if(errflg){
    return;
  } // end of errflg if
  
  // Mark all symbols
  mark();
  
  level--;
  
} // end of Block


// Declare a constant
void ConstDeclar(lexeme *list){
  int decl = 1;
  
  while(decl){
    // If their isn't an identifier
    // Then call error 2 and stop
    lIndex++;
    if(list[lIndex].type != identsym){
      printparseerror(2);
      errflg = 1;
      return;
    } // end of error 2 If
    
    // Copying the name of the constant
    char name[30];
    strcpy(name, list[lIndex].name);
    
    // If their isn't a ":=" 
    // Then call error 2 and stop
    lIndex++;
    if(list[lIndex].type != assignsym){
      printparseerror(2);
      errflg = 1;
      return;
    } // end of error 2 If
    
    // If it isn't a number 
    // Then call error 2 and stop
    lIndex++;
    if(list[lIndex].type != numbersym){
      printparseerror(2);
      errflg = 1;
      return;
    } // end of error 2 If
    
    // Set the value
    int val = list[lIndex].value;
    
    // If it hasn't already been declared
    // Add to the symbol table
    if(multipledeclarationcheck(name) == -1){
      addToSymbolTable(1, name, val, level, 0,0);
    } // end of multipledeclarationcheck If
    
    // Else it is already declared
    // So call error 18 and stop
    else{
      printparseerror(18);
      errflg = 1;
      return;
    } // end of error 18 else
    
    // If there isn't a comma
    lIndex++;
    if(list[lIndex].type != commasym){
      
      // If there is another declaration with no comma
      // Then call error 13 and stop
      if(list[lIndex].type == identsym){
        printparseerror(13);
        errflg = 1;
        return;
      } // end of error 13 If
      
      // If there is a semicolon
      // Then stop constant declaration
      else if(list[lIndex].type == semicolonsym){
        decl = 0;
      } // end of semicolonsym If
      
      // Else it wasn't a semicolon to end const
      // So call error 14 and stop
      else{
        printparseerror(14);
        errflg = 1;
        return;
      } // end of error 14 else 
    } // end of not Commasym If
  } // end of While
  
  lIndex++;
} // end of ConstDeclar


// Declare a variable
int VarDeclar(lexeme *list){
  int numDec = 0;
  int decl = 1;
  
  while(decl){
    // If their isn't an identifier
    // Then call error 3 and stop
    lIndex++;
    if(list[lIndex].type != identsym){
      printparseerror(3);
      errflg = 1;
      return 0;
    } // end of error 3 If
    
    // Copying the name of the constant
    char name[30];
    strcpy(name, list[lIndex].name);

    
    // If it hasn't already been declared
    // Add to the symbol table
    if(multipledeclarationcheck(name) == -1){
      addToSymbolTable(2, name, 0, level, numDec + 3, 0);
    } // end of multipledeclarationcheck If
    
    // Else it is already declared
    // So call error 18 and stop
    else{
      printparseerror(18);
      errflg = 1;
      return 0;
    } // end of error 18 else
    
    // If there isn't a comma
    lIndex++;
    if(list[lIndex].type != commasym){
      
      // If there is another declaration with no comma
      // Then call error 13 and stop
      if(list[lIndex].type == identsym){
        printparseerror(13);
        errflg = 1;
        return 0;
      } // end of error 13 If
      
      // If there is a semicolon
      // Then stop constant declaration
      else if(list[lIndex].type == semicolonsym){
        decl = 0;
      } // end of Semicolonsym If
      
      // Else it wasn't a semicolon to end const
      // So call error 14 and stop
      else{
        printparseerror(14);
        errflg = 1;
        return 0;
      } // end of error 14 else  
    } // end of not Commasym If
      
    // increment the list index
    numDec++;
  } // end of While

  lIndex++;
  return numDec;
} // end of VarDeclar 


// Declare a procedure
void ProcDeclar(lexeme *list){


  // If their isn't an identifier
  // Then call error 4 and stop
  lIndex++;
  
  
  if(list[lIndex].type != identsym){
    printparseerror(4);
    errflg = 1;
    return;
  } // end of error 4 If
  
  // Copying the name of the constant
  char name[30];
  strcpy(name, list[lIndex].name);
  
  // If there isn't a semicolon
  // Then call error 4 and stop
  lIndex++;

  if(list[lIndex].type != semicolonsym){
    printparseerror(4);
    errflg = 1;
    return;
  } // end of error 4 If 
  
  // If it hasn't already been declared
  // Add to the symbol table
  if(multipledeclarationcheck(name) == -1){  
    addToSymbolTable(3, name, 0, level, 0, 0);
  } // end of multipledeclarationcheck If
  
  // Else it is already declared
  // So call error 18 and stop
  else{
    printparseerror(18);
    errflg = 1;
    return;
  } // end of error 18 else
  
  lIndex++;

  Block(list);
  if(errflg){
    return;
  } // end of errflg if
  
  
  // RET
  emit(2,0,0);  
  
  // If there isn't a semicolon to end the procedure
  // Then call error 14 and stop
  if(list[lIndex].type != semicolonsym){
    printparseerror(14);
    errflg = 1;
    return;
  } // end of error 14 If 
  
  // increment the list index
  lIndex++;
  
} // end of ProcDeclar


void Statement(lexeme *list){  
  // Assignment statement
  // If there is an assignment symbol or variable symbol
  if(list[lIndex].type == identsym){
  
    int isVar = findsymbol(list[lIndex].name, 2);
    int isDecl = 1;
    
    if(findsymbol(list[lIndex].name, 1) == -1 && findsymbol(list[lIndex].name, 2) == -1 && findsymbol(list[lIndex].name, 3) == -1){
      isDecl = 0;
    }
    
    // If the symbol is not a variable
    // Then call error 6 and stop
    if(isVar == -1){
      printparseerror(6);
      errflg = 1;
      return;
    } // end of error 6 If
    
    // If symbol hasn't already been declared
    // Then call error 19 and stop
    else if(!isDecl){   
      printparseerror(19);
      errflg = 1;
      return;
    } // end of error 19 If
    
    
    // If there isn't an assignment symbol that follows variable
    // Then call error 5 and stop
    lIndex++;  
    if(list[lIndex].type != assignsym){
      printparseerror(5);
      errflg = 1;
      return;
    } // end of error 5 If
    
    lIndex++;

    Expression(list);
    if(errflg){
      return;
    }
    
    // STO
    emit(4, level - table[isVar].level, table[isVar].addr);
    RegSpace--; 
  } // end of Assignment If
  
  
  // Begin statement
  if(list[lIndex].type == beginsym){
  
    lIndex++;
    Statement(list);
    if(errflg){
      return;
    } // end of errflg if
    
    // While the current symbol isn't the end symbol
    while(list[lIndex].type == semicolonsym){
      lIndex++;
      Statement(list);
      if(errflg){
        return;
      } // end of errflg if
    } // end of While
 
    // If it isn't end but is "identifier, read, write, begin, call, if, or while" instead
    // Then call error 15 and stop
    if(list[lIndex].type != endsym && (list[lIndex].type == identsym || list[lIndex].type == readsym || list[lIndex].type == writesym || list[lIndex].type == beginsym || list[lIndex].type == callsym || list[lIndex].type == ifsym || list[lIndex].type == whilesym)){
      printparseerror(15);
      errflg = 1;
      return;
    } // end of error 15
    
    // If it isn't end and isn't "identifier, read, write, begin, call, if, or while" instead
    // Then call error 16 and stop
    if(list[lIndex].type != endsym && (list[lIndex].type != identsym || list[lIndex].type != readsym || list[lIndex].type != writesym || list[lIndex].type != beginsym || list[lIndex].type != callsym || list[lIndex].type != ifsym || list[lIndex].type != whilesym)){
      printparseerror(16);
      errflg = 1;
      return;
    } // end of error 16 
    
     lIndex++;
     
  } // end of Begin If
  
  
  // If Statement
  if(list[lIndex].type == ifsym){
    lIndex++;
    Condition(list);
    if(errflg){
      return;
    } // end of errflg if
    
    // If If is not followed by Then
    // Then call error 8 and stop
    if(list[lIndex].type != thensym){
     printparseerror(8);
     errflg = 1;
     return;
    } // end of error 8
    
    int jpcIndex = cIndex;
    // JPC
    emit(8, 0, 0);
    RegSpace--;
    
    lIndex++;
    Statement(list);
    if(errflg){
      return;
    } // end of errflg if
    
    // If token is elsesym
    if(list[lIndex].type == elsesym){
      int jmpIndex = cIndex;
      // JMP
      emit(7,0,0);
      code[jpcIndex].m = cIndex;
      
      lIndex++;
      Statement(list);
      if(errflg){
        return;
      } // end of errflg if
      
      code[jmpIndex].m = cIndex;
    }
    
    // If there is no else statement
    else{
      code[jpcIndex].m = cIndex;
    } // end of no else else
  } // end of If If
  
  
  // While statement
  if(list[lIndex].type == whilesym){
    int loopIndex = cIndex;
    
    lIndex++;
    Condition(list);
    if(errflg){
      return;
    } // end of errflg if
    
    // If While isn't followed by Do
    // Then call error 9 and stop
    if(list[lIndex].type != dosym){
      printparseerror(9);
      errflg = 1;
      return;
    } // end of error 9
    
    int jpcIndex = cIndex;
    // JPC
    emit(8, 0, 0);
    RegSpace--;
    
    lIndex++;
    Statement(list);
    if(errflg){
      return;
    } // end of errflg if
    
    // JMP
    emit(7,0,loopIndex);
    
    code[jpcIndex].m = cIndex;
  } // end of While if
  
  
  // Read Statement
  if(list[lIndex].type == readsym){    
    lIndex++;  
    int isVar = findsymbol(list[lIndex].name, 2);
    int isDecl = 1;
    
    if(findsymbol(list[lIndex].name, 1) == -1 && findsymbol(list[lIndex].name, 2) == -1 && findsymbol(list[lIndex].name, 3) == -1){
      isDecl = 0;
    } 
    // If the symbol is not a variable
    // Then call error 6 and stop
    if(isVar == -1){   
      printparseerror(6);
      errflg = 1;
      return;
    } // end of error 6 If
    
    // If symbol hasn't already been declared
    // Then call error 19 and stop
    else if(!isDecl){    
      printparseerror(19);
      errflg = 1;
      return;
    } // end of error 19 If

    emit(9, 0, 2); // RED
    RegSpace++;
    if(RegSpace > MAX_REG_HEIGHT){
      printparseerror(20);
      errflg = 1;
      return;
    }
    
    // STO
    emit(4, level - table[isVar].level, table[isVar].addr);
    
    lIndex++;
  } // End of Read if
  
  
  // Write statement
  if(list[lIndex].type == writesym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if 
    
    // WRT
    emit(9,0,1);
  } // end of Write If
  
  
  // Call statement
  if(list[lIndex].type == callsym){
    lIndex++;    
    int isPro = findsymbol(list[lIndex].name, 3);
    int isDecl = 1;
    
    if(findsymbol(list[lIndex].name, 1) == -1 && findsymbol(list[lIndex].name, 2) == -1 && findsymbol(list[lIndex].name, 3) == -1){
      isDecl = 0;
    }

    // If the symbol is a semicolon
    // Then call error 7 and stop
    if(list[lIndex].type == semicolonsym){
    printparseerror(7);
      errflg = 1;
      return;
    } // end of error 7 If

    // If symbol hasn't already been declared
    // Then call error 19 and stop
    if(!isDecl){
      printparseerror(19);
      errflg = 1;
      return;
    } // end of error 19 If
  
    // If the symbol is not a procedure
    // Then call error 7 and stop
    if(isPro == -1){
      printparseerror(7);
      errflg = 1;
      return;
    } // end of error 7 If
    
    // CAL
    emit(5,level - table[isPro].level, isPro);
        
    
    lIndex++;
    
  } // end of Call If
  
} // end of Statement



// Handles conditional Operators:
void Condition(lexeme *list){
  Expression(list);
  if(errflg){
    return;
  } // end of errflg if

  // Operator "=="
  if(list[lIndex].type == eqlsym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if
    // EQL
    emit(2, 0, 6);
    RegSpace--;
  } // End of eqlsym if

  // Operator "!="
  else if(list[lIndex].type == neqsym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if
    // NEQ
    emit(2, 0, 7);
    RegSpace--;
  } // End of neqsym if
    
  // Operator "<"
  else if(list[lIndex].type == lsssym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if
    // LSS
    emit(2, 0, 8);
    RegSpace--;
  } // End of lsssym if

  // Operator "<="
  else if(list[lIndex].type == leqsym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if    
    // LEQ
    emit(2, 0, 9);
    RegSpace--;
  } // End of leqsym if

  // Operator ">"
  else if(list[lIndex].type == gtrsym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if
    // GTR
    emit(2, 0, 10);
    RegSpace--;
  } // End of gtrsym if

  // Operator ">="
  else if(list[lIndex].type == geqsym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if    
    // GEQ
    emit(2, 0, 11);
    RegSpace--;
  } // End of geqsym if

  // Error for when there is no conditional operator in the if statement.
  else
  {
    printparseerror(10);
    errflg = 1;
    return;
  } // End of else
} // End of Condition


void Expression(lexeme *list){
  if(list[lIndex].type == minussym){
    lIndex++;
    Term(list);
    if(errflg){
      return;
    } // end of errflg if
    emit(2, 0, 1); // Emit NEG
    while(list[lIndex].type == plussym || list[lIndex].type == minussym){
      if(list[lIndex].type == plussym){
        lIndex++;
        Term(list);
        if(errflg){
          return;
        } // end of errflg if
        emit(2, 0, 2); // Emit ADD
        RegSpace--;
      } // End of if

      else{
        lIndex++;
        Term(list);
        if(errflg){
          return;
        } // end of errflg if
        emit(2, 0, 3); // Emit SUB
        RegSpace--;
      } // End of else
    } // End of while
  } // End of if

  else{
    if(list[lIndex].type == plussym){
      lIndex++;
    } // End of if

    Term(list);
    while(list[lIndex].type == plussym || list[lIndex].type == minussym){  
      if(list[lIndex].type == plussym){
        lIndex++; 
        Term(list);
        if(errflg){
          return;
        } // end of errflg if
        emit(2, 0, 2); // Emit ADD
        RegSpace--;
      } // End of if

      else{
        lIndex++; 
        Term(list);
        if(errflg){
          return;  
        } // end of errflg if
        emit(2, 0, 3); // Emit SUB
        RegSpace--;
      }
    } // End of while
  } // End of else

  if(list[lIndex].type == plussym || list[lIndex].type == minussym || list[lIndex].type == multsym || list[lIndex].type == divsym){
    printparseerror(17);
    errflg = 1;
    return;
  } // End of if
} // End of Expression


void Term(lexeme *list){
  Factor(list);
  if(errflg){
    return;
  } // end of errflg if
  while(list[lIndex].type == multsym || list[lIndex].type == divsym){
    if(list[lIndex].type == multsym){
      lIndex++;    
      Factor(list);
      if(errflg){
        return;
      } // end of errflg if
      emit(2, 0, 4); // Emit MUL
      RegSpace--;
    } // End of if

    else{
      lIndex++;
      Factor(list);
      if(errflg){
        return;
      } // end of errflg if
      emit(2, 0, 5); // Emit DIV
      RegSpace--;
    } // End of else
  } // End of while
} // End of Term


void Factor(lexeme *list){
  if(list[lIndex].type == identsym){
    if(findsymbol(list[lIndex].name, 3) != -1){
      printparseerror(11);
      errflg = 1;
      return;
    } // End of identsym error if

    int symIdx1 = findsymbol(list[lIndex].name, 1);
    int symIdx2 = findsymbol(list[lIndex].name, 2);

    if(table[symIdx1].level > table[symIdx2].level || symIdx2 == -1){
      emit(1, 0, table[symIdx1].val); // Emit LIT
      RegSpace++;
      if(RegSpace > MAX_REG_HEIGHT){
        printparseerror(20);
        errflg = 1;
        return;
      } // End of error if
    } // End of if

    else if(table[symIdx1].level < table[symIdx2].level || symIdx1 == -1){
      emit(3, level - table[symIdx2].level, table[symIdx2].addr); // Emit LOD
      RegSpace++;
      if(RegSpace > MAX_REG_HEIGHT){
        printparseerror(20);
        errflg = 1;
        return;
      } // End of error if
    } // End of else if
  } // End of identsym if

  else if(list[lIndex].type == numbersym){
    emit(1, 0, list[lIndex].value); // Emit LIT
    RegSpace++;
    if(RegSpace > MAX_REG_HEIGHT){
      printparseerror(20);
      errflg = 1;
      return;
    } // End of if
  } // End of numbersym if

  else if(list[lIndex].type == lparensym){
    lIndex++;
    Expression(list);
    if(errflg){
      return;
    } // end of errflg if    

    if(list[lIndex].type != rparensym){
      printparseerror(12);
      errflg = 1;
      return;
    }
  } // End of lparensym if

  // If the token is not a (, number, or identifier, then an error is thrown.
  else{
    printparseerror(11);
    errflg = 1;
    return;
  } // End of else

  lIndex++;
} // end of Factor


// adds a line of code to the program
void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

// add a symbol to the symbol table
void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}

// mark the symbols belonging to the current procedure, should be called at the end of block
void mark()
{
	int i;
	for (i = tIndex - 1; i >= 0; i--)
	{
		if (table[i].mark == 1)
			continue;
		if (table[i].level < level)
			return;
		table[i].mark = 1;
	}
}

// checks if a new symbol has a valid name, by checking if there's an existing symbol
// with the same name in the procedure
int multipledeclarationcheck(char name[])
{
	int i;
	for (i = 0; i < tIndex; i++)
		if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
			return i;
	return -1;
}

// returns the index of a symbol with a given name and kind in the symbol table
// returns -1 if not found
// prioritizes lower lex levels
int findsymbol(char name[], int kind)
{
	int i;
	int max_idx = -1;
	int max_lvl = -1;
	for (i = 0; i < tIndex; i++)
	{
		if (table[i].mark == 0 && table[i].kind == kind && strcmp(name, table[i].name) == 0)
		{
			if (max_idx == -1 || table[i].level > max_lvl)
			{
				max_idx = i;
				max_lvl = table[i].level;
			}
		}
	}
	return max_idx;
}

void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		case 20:
			printf("Parser Error: Register Overflow Error\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RET\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("EQL\t");
						break;
					case 7:
						printf("NEQ\t");
						break;
					case 8:
						printf("LSS\t");
						break;
					case 9:
						printf("LEQ\t");
						break;
					case 10:
						printf("GTR\t");
						break;
					case 11:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}