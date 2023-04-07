/* Authors: Ethan Ellis & D'Antae Aronne
   Course: COP3402-22Spring 0002
   Assignment: HW1 - VM
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define MAX_REG_LENGTH 10
#define MAX_DATA_LENGTH 50
#define MAX_PROGRAM_LENGTH 150

void print_execution(int line, char *opname, instruction IR, int PC, int BP, int SP, int RP, int *data_stack, int *register_stack)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.l, IR.m, PC, BP, SP, RP);
	
	// print register stack
	for (i = MAX_REG_LENGTH - 1; i >= RP; i--)
		printf("%d ", register_stack[i]);
	printf("\n");
	
	// print data stack
	printf("\tdata stack : ");
	for (i = 0; i <= SP; i++)
		printf("%d ", data_stack[i]);
	printf("\n");
}

int base(int L, int BP, int *data_stack)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = data_stack[rtn];
		ctr--;
	}
	return rtn;
}

void execute_program(instruction *code, int printFlag)
{
  int BP = 0;
  int SP = BP - 1;
  int PC = 0;
  int RP = MAX_REG_LENGTH;
  int line = PC;

  int dataStack[MAX_DATA_LENGTH] = {0};
  int registerStack[MAX_REG_LENGTH] = {0};

  int halt = 1;  
  instruction IR;

	if (printFlag)
	{
		printf("\t\t\t\tPC\tBP\tSP\tRP\n");
		printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, RP);
	}
	
  // Cycle through the instructions:
  while(halt == 1)
  {
    // Fetch instructions:
    IR = code[PC];
    PC++;

    // Execute instructions:
    // ISA:
    switch(IR.opcode)
    {
      // LIT:
      case 1:
      {
        RP--;
        registerStack[RP] = IR.m;
        print_execution(line, "LIT", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // OPR instructions:
      case 2:
      {
        switch(IR.m)
        {
          // RET:
          case 0:
          {             
            SP = BP - 1;
            PC = dataStack[SP + 3];
            BP = dataStack[SP + 2];
            print_execution(line, "RET", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // NEG:
          case 1:
          {
            registerStack[RP] *= -1;
            print_execution(line, "NEG", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // ADD:
          case 2:
          {
            RP++;
            registerStack[RP] = registerStack[RP] + registerStack[RP - 1];
            print_execution(line, "ADD", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }
          
          // SUB:
          case 3:
          {
            RP++;
            registerStack[RP] = registerStack[RP] - registerStack[RP - 1];
            print_execution(line, "SUB", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // MUL:
          case 4:
          {
            RP++;
            registerStack[RP] = registerStack[RP] * registerStack[RP - 1];
            print_execution(line, "MUL", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // DIV:
          case 5:
          {
            RP++;
            registerStack[RP] = registerStack[RP] / registerStack[RP - 1];
            print_execution(line, "DIV", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // EQL:
          case 6:
          {
            RP++;
            if(registerStack[RP] == registerStack[RP - 1])
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "EQL", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // NEQ:
          case 7:
          {
            RP++;
            if(registerStack[RP] != registerStack[RP - 1])
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "NEQ", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // LSS:
          case 8:
          {
            RP++;
            if(registerStack[RP] < registerStack[RP - 1])
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "LSS", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // LEQ:
          case 9:
          {
            RP++;
            if(registerStack[RP] <= registerStack[RP - 1])
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "LEQ", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // GTR:
          case 10:
          {
            RP++;
            if(registerStack[RP] > registerStack[RP - 1])
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "GTR", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // GEQ:
          case 11:
          {
            RP++;
            if(registerStack[RP] >= registerStack[RP - 1])
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "GEQ", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // AND
          case 12:
          {
            RP++;
            if(registerStack[RP] == 1 && registerStack[RP - 1] == 1)
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "AND", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // ORR
          case 13:
          {
            RP++;
            if(registerStack[RP] == 1 || registerStack[RP - 1] == 1)
            {
              registerStack[RP] = 1;
            }

            else
            {
              registerStack[RP] = 0;
            }

            print_execution(line, "ORR", IR, PC, BP, SP, RP, dataStack, registerStack);
            break;
          }

          // NOT
          case 14:
          {
            if(registerStack[RP] == 1)
            {
              registerStack[RP] = 0;
            }

            else if(registerStack[RP] == 0)
            {
              registerStack[RP] = 1;
            }

            print_execution(line, "NOT", IR, PC, BP, SP, RP, dataStack, registerStack);
          }
        }
        
        break;
      }

      // LOD:
      case 3:
      {
        RP--;
        registerStack[RP] = dataStack[base(IR.l, BP, dataStack) + IR.m];
        print_execution(line, "LOD", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // STO:
      case 4:
      {
        dataStack[base(IR.l, BP, dataStack) + IR.m] = registerStack[RP];
        RP++;
        print_execution(line, "STO", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // CAL:
      case 5:
      {
        dataStack[SP + 1] = base(IR.l, BP, dataStack);
        dataStack[SP + 2] = BP;
        dataStack[SP + 3] = PC;
        BP = SP + 1;
        PC = IR.m;
        print_execution(line, "CAL", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // INC:
      case 6:
      {
        SP += IR.m;
        print_execution(line, "INC", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // JMP:
      case 7:
      {
        PC = IR.m;
        print_execution(line, "JMP", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // JPC:
      case 8:
      {
        if(registerStack[RP] == 0)
        {
          PC = IR.m;          
        }
        
        RP++;
        print_execution(line, "JPC", IR, PC, BP, SP, RP, dataStack, registerStack);
        break;
      }

      // WRT / RED / HAL:
      case 9:
      {
        // WRT:
        if(IR.m == 1)
        {
          printf("Top of Stack Value: %d\n", registerStack[RP]);
          RP++;
          print_execution(line, "WRT", IR, PC, BP, SP, RP, dataStack, registerStack);
          break;
        }

        // RED:
        else if(IR.m == 2)
        {
          RP--;
          printf("Please Enter an Integer: \n");
          scanf("%d", &registerStack[RP]);                    
          print_execution(line, "RED", IR, PC, BP, SP, RP, dataStack, registerStack);
          break;
        }

        // HAL:
        else if(IR.m == 3)
        {
          halt = 0;
          print_execution(line, "HAL", IR, PC, BP, SP, RP, dataStack, registerStack);
          break;
        }
      }
    }

    line = PC;  
  }
}