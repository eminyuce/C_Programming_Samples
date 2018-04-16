
 ============================================================================
 Name        : OurShell.c
 Author      : emin yuce
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 * File:   newmain.c
 * Author: Yuce
 *
 * Created on 07 Aral�k 2008 Pazar, 14:27
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define BUFFER_SIZE 50
#define MAX_BG 10
#define NAME_SIZE 15


void ExecuteCV(char ** args) ;


// global variables
char pipeInput[MAX_LINE];

int count = 0; //Increment each input buffer..

int finished = 0;

int getArrayLength(char ** arr) {
    int n = 0;
    while (arr[n] != '\0') {
        n++;
    }
    return n;
}
/**
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters. setup() sets the args parameter as a
 * null-terminated string.
 */
void setup(char inputBuffer[], char *args[]) {
    int length, /* # of characters in the command line */
    i, /* loop index for accessing inputBuffer array */
    start, /* index where beginning of next command parameter is */
    ct; /* index of where to place the next parameter into args[] */

    ct = 0;

    /* read what the user enters on the command line */
        length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

    for (i = 0; i < length; i++) {
        pipeInput[i] = '\0';
    }
    strcpy(pipeInput, inputBuffer);
   
    count++;

    start = -1;

    if (length == 0)
        exit(0); /* ^d was entered, end of user command stream */
    if (length < 0) {
        perror("error reading the command\n");
        exit(-1); /* terminate with error code of -1 */
    }

    /* examine every character in the inputBuffer */
    for (i = 0; i < length; i++) {
        switch (inputBuffer[i]) {
        case ' ':
        case '\t': /* argument separators */
            if (start != -1) {
                args[ct] = &inputBuffer[start]; /* set up pointer */
                ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

        case '\n': /* should be the final char examined */
            if (start != -1) {
                args[ct] = &inputBuffer[start];
                ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

        default: /* some other character */
            if (start == -1)
                start = i;
            
        }
    }

    args[ct] = NULL; /* just in case the input line was > 80 */
}
int makeargv(const char *s, const char *delimiters, char ***argvp) {
    int error;
    int i;
    int numtokens;
    const char *snew;
    char *t;

    if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
        errno = EINVAL;
        return -1;
    }
    *argvp = NULL;
    snew = s + strspn(s, delimiters);
    if ((t = malloc(strlen(snew) + 1)) == NULL)
        return -1;
    strcpy(t, snew);
    numtokens = 0;
    if (strtok(t, delimiters) != NULL)
        for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++)
            ;

    if ((*argvp = calloc(1, ((numtokens + 1) * sizeof(char *)))) == NULL) {
        error = errno;
        free(t);
        errno = error;
        return -1;
    }

    if (numtokens == 0)
        free(t);
    else {
        strcpy(t, snew);
        **argvp = strtok(t, delimiters);
        for (i = 1; i < numtokens; i++)
            *((*argvp) + i) = strtok(NULL, delimiters);
    }
    *((*argvp) + numtokens) = NULL;
    return numtokens;
}
void ExecuteCV(char ** args) {

    char **ourArgv;
    int numberTokens = 0;
    char delimeters[] = ":";
    int i;
    char str[100];
    numberTokens = makeargv(getenv("PATH"), delimeters, &ourArgv);

    for (i = 0; i < numberTokens; i++) {
        strcpy(str, ourArgv[i]);
        strcat(str, "/");
        strcat(str, args[0]);
        if (access(str, F_OK) == 0) {
            if (execv(str, args) < 0) {
                printf("Failed\n");
            }
        }
    }
}
int main(void) {
    char inputBuffer[MAX_LINE]; /* buffer to hold the co mainmmand entered */
    char *args[MAX_LINE / +1]; /* command line (of 80) has max of 40 arguments */
    pid_t child_pid;
    char fullPath[100];
    int i;
    int pipeExist = 0;
    int redirExist = 0;
    char ** slashArray;
    char slashDel[] = "/";
    char ** myArray;

   



    while (1) { /* Program terminates normally inside setup */


        for (i = 0; i < MAX_LINE; i++) {
            
            inputBuffer[i] = '\0';
        }

        printf("COMMAND->\n");

        setup(inputBuffer, args);


            /* get next command */
            if (args[0] != NULL) {
                i = 0;
                if (strncmp(args[0], "cd", 2) == 0) {
                    if (args[1] == NULL) {
                        //If the directory is not present,report the current directory.
                        printf(
                                "You must enter a path to change current directory..\n");
                    } else if (chdir(args[1]) == 0) {
                        printf("Changing current directory\n");
                    } else {
                        printf("Directory doesn't exists\n");
                    }
                }else if (strncmp(args[0], "exit", 4) == 0) {
                        exit(0);
                }else{
                    
                    child_pid = fork();

                    if (child_pid < 0) {
                        printf("Fork Failed.\n");
                        exit(1);
                    } else if (child_pid == 0) {
                     
                            //If user enters command, we find command's path and then execute it.
                            ExecuteCV(args);
                            
                        
                    } else if (child_pid > 0) {
			//The shell waits for the new
			//process to terminate before accepting another command.
                        waitpid(child_pid, NULL, 0);     
                    }

                }

        }
    }
}

