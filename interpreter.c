#include <stdio.h>
#include "machine.h"
#include "interpreter.h"

int load_program(Machine *const spim, const Word program[], int program_size) {
  int i;

  if(spim == NULL || program == NULL || (program_size >12288) ){
    return 0;
  }

  i=0;
  while (i<program_size){
    spim->memory[i] = program[i];
    
    i++;
  }
  spim->registers[11]=0;
    
  return 1;
}
Status run_SPIM_program(Machine *const spim, int max_instr,
                        int *const num_instr, int trace_flag) {

  unsigned int instruction, w,  opcode, address;
  int reg1, reg2, reg3, temp;
  w=0;
  temp = 0;

  *num_instr = w;
  
  if (spim==NULL || num_instr == NULL) {
    return PARAMETER_ERROR;
  }
  if (max_instr < 1)
    return TIMEOUT;




  while (w < max_instr){
 
    instruction = spim->memory[spim->registers[11]/4];
    
    if (!valid_instruction(instruction)){
      return INVALID_INSTRUCTION;
    }

    /*Extract information from bit shifting
     */
    opcode = (instruction >> 28);
    address = (instruction << 16) >> 16;
    reg1 = (instruction << 4) >> 28;
    reg2 = (instruction << 8) >> 28;
    reg3 = (instruction << 12) >> 28;

    switch (opcode) {
    
    case 0: /*HALT*/
      *num_instr=w;
      return HALTED;
      break;
      
    case 1:/*ADD*/
      
      spim->registers[reg1]= (spim->registers[reg2]+spim->registers[reg3]);
      spim->registers[11]+=4;
      break;
    
    case 2:/*MUL*/
      spim->registers[reg1]= (spim->registers[reg2]*spim->registers[reg3]);
      spim->registers[11]+=4;
      break;

    case 3:/*NEG*/

      spim->registers[reg1]= (-1*spim->registers[reg2]);
      spim->registers[11]+=4;
      break;
    
    case 4:/*SHL*/

      spim->registers[reg1]= (spim->registers[reg2]<<address);
      spim->registers[11]+=4;
      break;
    
    case 5:/*AND*/
      if (spim->registers[reg2]*spim->registers[reg3] != 0){
	spim->registers[1] = 0;
      }
      else{
	spim->registers[1]=1;
      }
      spim->registers[11]+=4;
      break;
    
    case 6: /*NOT*/
      if (spim->registers[reg2] != 0){
	spim->registers[1] = 0;
      }
      else{
	spim->registers[1]=1;
      }
   
      spim->registers[11]+=4;
      break;
    
    case 7:/*LW*/
      spim->registers[reg1]=spim->memory[address/4];
      spim->registers[11]+=4;
      break;
    
    case 8:/*LI*/

      spim->registers[reg1]=address;
      spim->registers[11]+=4;
      break;

    case 9:/*SW*/

      spim->memory[address/4] = spim->registers[reg1];
      spim->registers[11]+=4;
      break;

    case 10:/*MOVE*/

      spim->registers[reg1] = spim->registers[reg2];
      spim->registers[11]+=4;
      break;

    case 11:/*BNE*/

      if (spim->registers[reg1]==spim->registers[reg2]){
	spim->registers[11]+=4;
      }
      else {
	spim->registers[11] = address;
      }
      
      break;

    case 12:/*READ*/
      scanf("%d",&temp);
      spim->registers[reg1] = temp;
      spim->registers[11]+=4;
      break;
    
    case 13:/*WRITE*/
      printf("%d\n", spim->registers[reg1]);
      spim->registers[11]+=4;
      break;
    }
    w++;
    *num_instr = w;

  }
  
  *num_instr = w;
  
  return TIMEOUT;
}


int reset(Machine *const spim) {
  if (spim != NULL){
    spim->registers[11]=0;
    return 0;
  }
  else {
    return 0;
  }
}
