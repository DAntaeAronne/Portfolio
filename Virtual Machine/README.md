Written in: C    
   
In my System Software class, we were given the semester long goal of creating a virtual machine (VM), lexical analyzer (LA), recursive descent parser (RDP) and intermediate code generator (ICG) for PL/0 in a way that all works together. Each source code is available above. The descriptions below will go in order of use.  
   
Lex.c:   
The LA which reads in a source program written in PL/0 and produces either the lexeme table of the source program along and list of tokens, or any error in the source that it identified. Though easily the simplest of the other files in this folder, it was still an important piece as it made the list of tokens which would be used by the parser.    
   
Parser.c:   
Next is the RDP and ICG which takes in the output produced by the LA. The RDP portion checks to see if there are any grammatical errors with the source program, in which it will print out the error found, if any, and stop. Another job is to fill out the symbol table, which contains all of the variables, procedure, and constants names within the PL/0 program. If no error is found, then the program is considered synthetically accurate and executes the ICG. From here, the symbol table and list of tokens are used to translate the source program into proper instructions for the VM as assembly code.


VM.c:   
Though it was the first created it is to be used at the end of the process, taking in the assembly code produced by the ICG. The VM which is a P-Code Machine (PM/0). It works as a stack machine with 2 memory areas for the "text", holding the instructions for the VM to execute, and the "stack", which is organized as a data-stack to store variables that grow upwards. There is also a downward growing register stack that executed arithmetic and logic instructions. This consists of several registers to handle the stacks: base pointer (BP), stack pointer (SP), program counter (PC), register pointer (RP), and instruction register (IR). The PM/0 runs its instruction cycle in 2 steps: "fetch", where the PC points to an instruction pulled out of the "text" segment and places it in the IR followed by incrementing the PC to point to the next instruction; and "execute", in which the instruction in the IR is executed using the data and register stacks.      
