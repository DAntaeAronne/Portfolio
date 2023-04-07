Written in: C    
   
In my System Software class we were given the semester long goal of creating a virtual machine (VM), lexical analyzer (LA), recursive descent parser (RDP) and intermediate code generator (ICG) for PL/0 before implementing them all together. Each source code is available above.   
   
Though the first created was the VM which would be a P-Code Machine (PM/0), this was to be implemented at the end of all of them working together. It would work as a stack machine with 2 memory areas for the "text", holding the instrucitons for the VM to execute, and the "stack", which is organized as a data-stack to store variables that grows upwards. There was also a downward growing register stack that executed arithmetic and logic instructions. This consisted of several registers to handle the stacks: base pointer (BP), stack pointer (SP), program counter (PC), register pointer (RP), and instruction register (IR). The PM/0 ran in its instruction cycle in 2 steps: "fetch", where the PC pointed to an instruction pulled out of the "text" segment and placed it in the IR followed by incrementing the PC to point to the next instruction; and "execute", in which the instruction in the IR was executed using the data and register stacks.    
    
Next came the lexical analyzer to read in a source program written in PL/0. It would them produce either the lexeme table of the source program along and list of tokens, or any error in the source that it identified. Though easily the most simple of the other files in this folder, it was still an important piece as it made the list of tokens which would be used by the parser.   
   
After is the RDP and ICG.
