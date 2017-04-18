/* Aleksander Sankar

*/

#include <stdio.h>

/*
  This program reads from a file using input redirection and prints out the number of characters in each
  line.
*/
int main() {

  /*This is the char representing the current character that we are dealing with
   */
  char current;

  /*Count stores how long each line appears (taking into account tab characters).
    Index keeps track of the actual number of characters in each line and is used for storing each
    character in the array.
  */
  int count = 0;
  int index = 0;

  /*This is the array that characters are stored in (this does not exceed 1000 in any line
   */
  char line[1000];

  /*Other counter variables for iterating through arrays and lines
   */
  int i;
  int j;
  

  /* Scan lines char by char and store into an array
   */
  current = 0;
  index = count;
  while (scanf("%c", &current) == 1) {/*while there are still characters to be read*/
    while (current != '\n') {/*iterate through each line*/
  

      /*Store the current char in the array
       */
      line[index] = current;
      index++;/*move to the next position in the array*/
    
      scanf("%c", &current);/*read the next character*/

      /*if a tab is encountered, adjust for how long the line actually appears, otherwise increment the
	line length by one
      */
    
      if (current == '\t')
	count += (7-count % 8);
      else
	count++;
    

    }
    
    printf("%4d: ", count);/*adds padding to the printed number and prints the length*/

    /*read from the array and print the contents of the line
     */
    for (i = 0; i < index; i++) {
      printf("%c", line[i]);
    }
    printf("\n");

    /*reset the counters and clear the array in preparation for the next line
     */
    count = 0;
    index = count;
    for (j = 0; j < 1000; j++) {
      line[j] = 0;
    }
  
  }
    
  return 0;
}
