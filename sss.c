/* Aleksander Sankar

   This project simulates the student submit server by executing commands
   to compile and run C programs. The compilation commands are read from
   one file and the test commands are read from a separate file and they 
   are both stored in a Commands struct consisting of two dynamic arrays
   of strings (represented as char pointers). The commands are read and 
   executed through the use of parent/child processes.
*/

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sysexits.h>
#include "sss.h"
#include "split.h"

/*This function reads in the compilation commands and test commands
  from files with file names given as parameters. It then stores them
  in the Commands struct and returns that struct. The Commands struct
  uses dynamic memory as well, so that many compilation commands or
  test commands can be read into it.
*/
Commands read_commands(const char *compile_cmds, const char *test_cmds) {
  Commands comm;
  char **c;
  char **t;
  int i;
  int max_length;
  int new_size;
  int first_alloc;
  FILE *fp;
  FILE *fp2;

  /*If either file name is null, exit*/
  if (compile_cmds == NULL || test_cmds == NULL)
    exit(0);

  /*The max length of each line is guaranteed to be under 257,
    including the null termination char*/
  max_length = 257;
  /*start with an allocation of size 20 (can be increased if needed)*/
  first_alloc = 20;

  /*Allocate memory for the array and check to see if it succeeded*/
  c =  malloc(sizeof(char*) * first_alloc);
  if (c == NULL) {
    perror("Allocation failed.\n");
    exit(0);
  }

  /*Open the file containing compile commands in read mode and exit 
    if not found*/
  fp = fopen(compile_cmds, "r");
  if (fp == NULL) {
    perror("Error opening file.\n");
    exit(0);
  }

  /*Allocate memory for the first string and check to see if it
    succeeded*/
  c[0] = malloc(max_length * sizeof(char));
  if (c[0] == NULL) {
    perror("Allocation failed.\n");
    exit(0);
  }

  /*Increment through each line of the file and store each one 
    into an entry of the string array*/
  i = 0;
  while (fgets(c[i], max_length ,fp) != NULL) {
    
    i++;
    /* check if we have gone over the allocated
       amount of memory */
    if (i >= first_alloc) {

      /* Double the allocation and reallocate */
      new_size = first_alloc * 2;

      /*Reallocate memory for the array and check to see if it 
	succeeded*/
      c = realloc(c, sizeof(char*) * new_size);
      if (c == NULL) {
	perror("Allocation failed 5.\n");
	exit(0);
      }
      first_alloc = new_size;
    }
    
    /* Allocate space for the next line and check to see if it
       succeeded */
    c[i] = malloc(max_length * sizeof(char));
    if (c[i] == NULL) {
      perror("Allocation failed 4.\n");
      exit(0);
    }
  }

  /*set the last element of the array to null*/
  free(c[i]);
  c[i] = NULL;

  /* Close file */
  fclose(fp);

    
  /*Essentially repeat process for reading the compilation
    commands*/

  first_alloc = 20;

  /*Allocate memory for the array and check to see if it
    succeeded*/
  t = malloc(sizeof(char*) * first_alloc);
  if (t == NULL) {
    perror("Allocation failed 3.\n");
    exit(0);
  }

  /*Open the file containing test commands in read mode
    and exit if not found*/
  fp2 = fopen(test_cmds, "r");
  if (fp2 == NULL) {
    printf("Error opening file.\n");
    exit(0);
  }
  
  /* Allocate space for the first line and check to see if it
     succeeded */
  t[0] = malloc(max_length * sizeof(char));
  if (t[0] == NULL) {
    perror("Allocation failed 67.\n");
    exit(0);
  }

  /*Increment through each line of the file and store each one
    into an entry of the string array*/
  i = 0;
  while (fgets(t[i], max_length ,fp2) != NULL) {
    i++;
      
    /* check if we have gone over the allocated amount
       of memory */
    if (i >= first_alloc) {

      /* Double the allocation and reallocate */
      new_size = first_alloc * 2;

      /*Reallocate memory for the array and check to see if it
	succeeded*/
      t = realloc(t, sizeof(char*) * new_size);
      if (t == NULL) {
	perror("Allocation failed 1.\n");
	exit(0);
      }
      first_alloc = new_size;
    }
    /* Allocate space for the next line and check to see if it
       succeeded */
    t[i] = malloc(max_length * sizeof(char));
    if (t[i] == NULL)	{
      perror("Allocation failed 2.\n");
      exit(0);
    }
  }

  /*set the last element of the array to null*/
  free(t[i]);
  t[i] = NULL;
  
  /* Close file */
  fclose(fp2);

  /*Set the Commands struct to the arrays we just made*/
  comm.compile = c;
  comm.test = t;

  return comm;
    
}

/*This function releases all of the dynamically allocated array
  used by the Commands struct so that memory leaks will not occur
*/
void clear_commands(Commands *const commands) {
  char **c;
  char **t;
  char *temp;
  int i;

  i = 0;
  /*cannot deallocate a null array*/
  if (commands != NULL) {
    c = commands -> compile;
    t = commands -> test;
    /*iterate though each element of the array and free it,
      using temporary
      pointers*/
    while (c[i] != NULL) {
      temp = c[i];
      free(temp);
      i++;
     
    }
    /*free the array itself*/
    free(c);

    /*iterate though each element of the array and free it,
      using temporary
      pointers*/
    i = 0;
    while (t[i] != NULL) {
      temp = t[i];
      free(temp);
      i++;
     
    }
    /*free the array itself*/
    free(t);
  }
}

/* This function uses the Commands struct parameter and executes
   all of the compilation commands that it contains. It forks each
   time and checks the return values of each process in order to
   determine if the command executed successfully. If one is 
   unsuccessful, the function returns zero. If they all execute
   successfully, the function returns one. 
*/
int compile_program(Commands commands) {
  pid_t pid;
  int status;
  char ** c;
  char **t;
  char *temp;
  int i;
  int j;

  /*This is the array of compilation commands*/
  c = commands.compile;
  i = 0;
 
  /*Iterate through each line and use split() to parse the 
    information so it can be executed*/
  while (c[i] != NULL) {
    t = split(c[i]);
    pid = fork();
    
    if (pid > 0) {  /* parent process*/
      /*wait for the child process to complete*/
      wait(&status);

      /*free the memory of the temporary array that stores the
	arguments of the command as strings*/
      j = 0;
      while (t[j] != NULL) {
	temp = t[j];
	free(temp);
	j++;
     
      }
      /*free the array itself*/
      free(t);

      /*if the process did not execute successfully, there is an
	error with that command so return 0*/
      if (WIFEXITED(status) == 0)
	return 0;
      else
	/*if the process's exit status is nonzero, it also failed,
	  so return zero*/
	if (WEXITSTATUS(status) != 0)
	  /*failed*/
	  return 0;

    } else  if (pid == 0) {  /* child */      
      /*execute the compilation command*/
      execvp(t[0], t);
      
    } else
      /*the fork was not successful*/
      return 0;

    /*move on to the next command*/
    i++;
    
  }
  /*function reaches here if all of the commands executed
    successfully*/
  return 1;
}

/* This function uses the Commands struct parameter and executes 
   all of the test commands that it contains. It forks each time and 
   checks the return values of each process in order to determine if
   the command executed successfully. If one is successful, the counter
   is incremented. If not, the counter does not increment, and the 
   function returns the number of successful tests. 
*/
int test_program(Commands commands) {
  pid_t pid;
  int status;
  char ** c;
  char **t;
  char *infile;
  char *outfile;
  char *temp;
  int input;
  int output;
  int i;
  int j;
  int fp_in;
  int fp_out;
  int passed;

  /*This is the array of test  commands*/
  c = commands.test;
  i = 0;
  passed = 0;

  /*these two ints store whether or not input or output redirection
    is used*/
  input = 0;
  output = 0;
 
  /*Iterate through each line and use split() to parse the
    information so it can be executed*/
  while (c[i] != NULL) {
    t = split(c[i]);
     
    /*IO redirection handling*/

    /*first find the number of tokens in the line for the test
      commands*/
    j = 0;
    while (t[j] != NULL) {
      j++;
    }
    
    /*check whether redirection of some form is used*/
    if ( j >= 3 &&  (strcmp(t[j-2], ">") == 0
		     || strcmp(t[1], "<") == 0)) {

      /*case for when both input and output redirection are used*/
      if (j >= 5) {
	/*set the boolean-resembling ints to 1 (true)*/
	input = 1;
	output = 1;
	/*allocate memory for the name of the file that the command 
	  is using for output*/
	outfile = malloc(sizeof(char) * (strlen(t[j-1]) + 1));
	if (outfile  == NULL) {
	  perror("Allocation failed.\n");
	  exit(0);
	}

	/*copy the name of the file over from the array of tokens*/
	strcpy(outfile, t[j-1]);
	/*open the file so that it is write-only, and set the 
	  corresponding file pointer to it*/
	fp_out = open(outfile, O_WRONLY);

	/*allocate memory for the name of the file that the command is
	  using for input*/
	infile = malloc(sizeof(char) * (strlen(t[j-3]) + 1));
	if (infile  == NULL) {
	  perror("Allocation failed.\n");
	  exit(0);
	}

        /*copy the name of the file over from the array of tokens*/
	strcpy(infile, t[j-3]);
	/*open the file so that it is read-only, and set the corresponding
	  file pointer to it*/
	fp_in = open(infile, O_RDONLY);

	/*free the last four elements of the array because we are not 
	  using them anymore*/
	free(t[j-4]);
	free(t[j-3]);
	free(t[j-2]);
	free(t[j-1]);

	/*set the last element to null*/
	t[j-4] = NULL;

	/*free the strings containing the file names*/
	free(infile);
	free(outfile);
	
      }

      /*case if only output redirection is used*/
      else if (strcmp(t[j-2], ">") == 0) {
   
	output = 1;

	/*allocate memory for the name of the file that the command is
	  using for output*/
	outfile = malloc(sizeof(char) * (strlen(t[j-1]) + 1));
	if (outfile == NULL) {
	  perror("Allocation failed.\n");
	  exit(0);
	}
	/*copy the name of the file over from the array of tokens*/
	strcpy(outfile, t[j-1]);
	/*free the elements of the array that we are not using anymore*/
	free(t[j-2]);
	free(t[j-1]);
	/*set the last element to null*/
	t[j-2] = NULL;
	/*open the file so that it is write-only, and set the corresponding
	  file pointer to it*/
	fp_out = open(outfile, O_WRONLY);
	/*free the string containing the file name*/
	free(outfile);
	
      }

      /*case if only input redirection is used*/
      else if (strcmp(t[1], "<") == 0) {

	input = 1;
	/*allocate memory for the name of the file that the command is
	  using for input*/
	infile = malloc(sizeof(char) * (strlen(t[2]) + 1));
	if (infile == NULL) {
	  perror("Allocation failed.\n");
	  exit(0);
	}
	
	/*copy the name of the file over from the array of tokens*/
	strcpy(infile, t[2]);
	/*free the elements of the array that we are not using anymore*/
	free(t[1]);
	free(t[2]);
	/*set the last element to null*/
	t[1] = NULL;

	/*open the file so that it is read-only, and set the corresponding 
	  file pointer to it*/
	fp_in = open(infile, O_RDONLY);
	/*free the string containing the name of the file */
	free(infile);
	
      }
      
    }
    
    pid = fork();
    
    if (pid > 0) {  /* parent process */
      /*wait for the child process to complete*/
      wait( &status);

      /*free the memory of the temporary array that stores the arguments
	of the command as strings*/
      j = 0;
      while (t[j] != NULL) {
	temp = t[j];
	free(temp);
	j++;
     
      }

      /*free the array itself*/
      free(t);
      
      /*if the process did not execute successfully, don't increment the
	counter*/
      if (WIFEXITED(status) == 0)
	passed += 0; /*silly code*/
      else
	if (WEXITSTATUS(status) == 0)
	  /*success, so increment the counter*/
	  passed++;
	else
	  /*failed, so do not increment the counter*/
	  passed += 0;

    }
    else if (pid == 0) {  /* child process*/

      /*if using input redirection, send info from the file pointer to
	the standard input*/
      if (input) {
	dup2(fp_in, STDIN_FILENO);
      }
      
      /*if using output redirection, send info from the file pointer to 
	the standard output*/
      if (output) {
	dup2(fp_out, STDOUT_FILENO);
      }
      /*execute the test command using the string token arguments*/
      execvp(t[0], t);
      
    } else /* if the for was unsuccessful*/
      return 0;

    i++; /*move to the next line's commands*/
  }
  /*return how many tests "passed"*/
  return passed;
}
