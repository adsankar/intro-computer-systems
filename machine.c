#include <stdio.h>
#include "machine.h"

/* Aleksander Sankar
*/

/* This function takes in a word and uses bit shifting operations to
   print out the relevant information including the opcode, which
   registers it uses, and the address, if it uses one
*/
void print_instruction(Word instruction) {

  /*Extract information from bit shifting
   */
  int opcode = instruction >> 28;
  int address = (instruction << 16) >> 16;
  int reg1 = (instruction << 4) >> 28;
  int reg2 = (instruction << 8) >> 28;
  int reg3 = (instruction << 12) >> 28;

  /*Handle each case individually and print the relevant
    information for each one. Also adds the appropriate formatting
  */
  switch (opcode) {
  case 0:
    printf("%s\n", "halt");
    break;
    
  case 1:
    printf("%s\tR%02d\tR%02d\tR%02d\n", "add", reg1, reg2, reg3);
    break;
    
  case 2:
    printf("%s\tR%02d\tR%02d\tR%02d\n", "mul", reg1, reg2, reg3);
    break;

  case 3:
    printf("%s\tR%02d\tR%02d\n", "neg", reg1, reg2);
    break;
    
  case 4:
    printf("%s\tR%02d\tR%02d\t%05d\n", "shl", reg1, reg2, address);
    break;
    
  case 5:
    printf("%s\tR%02d\tR%02d\tR%02d\n", "and", reg1, reg2, reg3);
    break;
    
  case 6:
    printf("%s\tR%02d\tR%02d\n", "not", reg1, reg2);
    break;
    
  case 7:
    printf("%s\tR%02d\t%05d\n", "lw", reg1, address);
    break;
    
  case 8:
    printf("%s\tR%02d\t%05d\n", "li", reg1, address);
    break;

  case 9:
    printf("%s\tR%02d\t%05d\n", "sw", reg1, address);
    break;

  case 10:
    printf("%s\tR%02d\tR%02d\n", "move", reg1, reg2);
    break;

  case 11:
    printf("%s\tR%02d\tR%02d\t%05d\n", "bne", reg1, reg2, address);
    break;

  case 12:
    printf("%s\tR%02d\n", "read", reg1);
    break;
    
  case 13:
    printf("%s\tR%02d\n", "write", reg1);
    break;

  }
  
}

/*Reads several words from the array and prints out information about them
 */
int disassemble(const Word memory[], int starting_address, int num_words) {
  int count = 0;
  Word current;

  /*Check to see if the parameters are valid, and return -1 if not
   */
  if ((starting_address % 4 != 0 && starting_address > 0 ) || memory == NULL ||
      (starting_address + 4 * num_words > 12284)) {
    return -1;
  }

  /*Iterate through and print the words
   */
  while (count < num_words) {
    current = memory[count + starting_address / 4];
    current = current | (4 * count + starting_address);

    if (valid_instruction(current)) {
      printf("0x%04x: ", (current << 16) >> 16);
      print_instruction(current);
      count++;
    }
    /*print the number of words written*/
    else return count;
  }
  
  return count;
}

/*Constructs a word from the paramaters using bitwise operations
  and then stores them as a word
*/
int encode_instruction(unsigned int opcode, unsigned int reg1,
		       unsigned int reg2, unsigned int reg3,
		       unsigned int memory_addr, Word *const instruction) {

  /*create a temporary value and add all bits
   */
  Word temp = 0;
  temp = temp | (opcode << 28) | (reg1 << 24) | (reg2 << 20) | (reg3 << 16)
    | memory_addr;
  /*if the parameters generate a valid word
   */
  if (valid_instruction(temp) && instruction != NULL) {

    /*these cases remove bits (set them to zero) that are not needed 
      for that operation
    */
    if (opcode == 0) {
      temp = temp & (15 << 28);
    }
    else if (opcode == 1 || opcode == 2 || opcode == 5) {
      temp = temp & (0xFFFF  << 16);
    }
    else if (opcode == 3 || opcode == 6 || opcode == 10) {
      temp = temp & (0xFFF << 20);
    }
    else if (opcode == 4 || opcode == 11) {
      temp = temp & (0xFFFF | (0xFFF << 20));
    }
    else if (opcode == 7 || opcode == 8 || opcode == 9) {
      temp = temp & (0xFFFF | (0xFF << 24));
    }
    else if (opcode  == 12 || opcode == 13) {
      temp = temp & (0xFF << 24);
    }

    /*once completed, set the pointer to it and return 1
     */
    *instruction = temp;
    return 1;
  }

  return 0;
}

/*Helper function that makes sure the register is not
  the first or last one, because they are unmodifiable 
*/
int illegal_mod(int reg) {
  if (reg == 0 || reg == 11) {
    return 1;
  }
  else return 0;
}

/*Checks to see if the address is valid in the SPIM
 */
int invalid_address(int address) {
  if (address > 49151 || (address % 4 != 0)) {
    return 1;
  }
  else return 0;
}

/*Helper function which makes sure the register number
  is a valid one
*/
int invalid_reg(int reg) {
  if (reg > 11) {
    return 1;
  }
  else return 0;
}

/*This function looks at the bits of the word and determines
  if the word is a valid instruction
*/
int valid_instruction(Word instruction) {
  /*separate out the useful pieces of bit information
   */
  int opcode = instruction >> 28;
  int address = (instruction << 16) >> 16;
  int reg1 = (instruction << 4) >> 28;
  int reg2 = (instruction << 8) >> 28;
  int reg3 = (instruction << 12) >> 28; 

  if (opcode > 13) {
    return 0;
  }

  /*Go through each case and specifically check if that word's bits
    match the opcode in it
  */
  switch (opcode) {
  case 0:
    /*nothing here*/
    break;

    /*check to see if any of the registers are invalid or if it is
      trying to modify a constant value
    */
  case 1:
    if (invalid_reg(reg1) || invalid_reg(reg2) || invalid_reg(reg3)
	|| illegal_mod(reg1)) {
      return 0;
    }
    break;
    
  case 2:
    if (invalid_reg(reg1) || invalid_reg(reg2) || invalid_reg(reg3)
	|| illegal_mod(reg1)) {
      return 0;
    }
    break;

  case 3:
    if (invalid_reg(reg1) || invalid_reg(reg2) || illegal_mod(reg1)) {
      return 0;
    }
    break;
    
  case 4:
    if (invalid_reg(reg1) || invalid_reg(reg2) || illegal_mod(reg1)
	|| (address > 49151)) { /*make sure the address is in range*/
      return 0;
    }
    /*check to see if the shift is in range*/
    if ( ((instruction << 24) >> 24) > 31) {
      return 0;
    }
    break;
    
  case 5:
    if (invalid_reg(reg1) || invalid_reg(reg2) || invalid_reg(reg3)
	|| illegal_mod(reg1)) {
      return 0;
    }
    break;
    
  case 6:
    if (invalid_reg(reg1) || invalid_reg(reg2) || illegal_mod(reg1)) {
      return 0;
    }
    break;
    
  case 7:
    if (invalid_reg(reg1) || illegal_mod(reg1)|| invalid_address(address)) {
      return 0;
    }
    break;
    
  case 8:
    if (invalid_reg(reg1) || illegal_mod(reg1) ||
	(address > 49151)) {
      return 0;
    }
    break;

  case 9:
    if (invalid_reg(reg1) || illegal_mod(reg1) ||
	invalid_address(address)) {
      return 0;
    }
    break;

  case 10:
    if (invalid_reg(reg1) || invalid_reg(reg2) || illegal_mod(reg1)) {
      return 0;
    }
    break;

  case 11:
    if (invalid_reg(reg1) || invalid_reg(reg2) || illegal_mod(reg1) ||
	invalid_address(address)) {
      return 0;
    }
    break;

  case 12:
    if (invalid_reg(reg1) || illegal_mod(reg1)) {
      return 0;
    }
    break;
    
  case 13:

    if (invalid_reg(reg1) || illegal_mod(reg1)) {
      return 0;
    }
    break;

  }
  
  return 1;
}

