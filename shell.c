#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define INPUT_STRING_SIZE 80

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"

int cmd_quit(tok_t arg[]) {
  printf("\n Bye \n" );
  exit(0);
  return 1;
}

int cmd_cd(tok_t arg[]) {
    
printf("change directory to %s \n",*arg);

char *directory = "/tempdir";
int ret;


ret = chdir (*arg);

 if (ret == 0){
   printf("Directory succesfully changed\n");
 }
 else{
 printf("error in changing directory : %s\n",strerror(errno));
 }
char cwd[1000];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       fprintf(stdout, "Current directory: %s\n", cwd);
   else
       perror("getcwd() error");
   return 0;printf("Bye" );
  exit(0);
  return 1;
}



int cmd_pwd(tok_t arg[]){

  char cwd[1000];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       fprintf(stdout, "Current directory: %s\n", cwd);
   else
       perror("getcwd() error");
   return 0;

}



 

int cmd_help(tok_t arg[]);


/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc; 
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "shows this help menu"},
  {cmd_quit, "quit", "quits the command shell"},
  {cmd_pwd,"pwd","displays the current directory"},
  {cmd_cd,"cd","changes directory"},
 
};

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */

void add_process(process* p)
{
  printf("well hello");
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
  /** YOUR CODE HERE */
  return NULL;
}

int cmd_fork(tok_t arg[]) {
size_t size = 120;
  int i, status = 0;
  pid_t childPid;
  char *nargs = (char *) malloc(size);

  if ((childPid = fork()) == 0) {
    //printf("arg0: %s, arg1: %s\n", arg[0], arg[1]);
    for (i=1; i < MAXTOKS && arg[i]; i++) {
      if (i > 1) {
        strcat(nargs, " ");
      }
      strcat(nargs, arg[i]);
      //printf("Nargs: %s\n", nargs);
    }
    execl(arg[0], arg[0], nargs, NULL);
    _exit(127);
  }

  if (childPid > 0) {
    waitpid(childPid, &status, 0);    
    //printf("Exit status of process was %d\n", status);
  } else {

    printf("Child failed");
  }

  return 0;

}


int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
  tok_t *t;			/* tokens parsed from input */
  int lineNum = 0;
  int fundex = -1;
  pid_t pid = getpid();		/* get current processes PID */
  pid_t ppid = getppid();	/* get parents PID */
  pid_t cpid, tcpid, cpgid;
https://onedrive.live.com/about/en-us/#
  init_shell();

  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

  lineNum=0;
  fprintf(stdout, "%d: ", lineNum);
  while ((s = freadln(stdin))){
    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if(fundex >= 0) cmd_table[fundex].fun(&t[1]);
    else {
      //fprintf(stdout, "This shell only supports built-ins. Replace this to run programs as commands.\n");
      
       cmd_fork(t);

 
    }
    fprintf(stdout, "%d: ", lineNum);
  }
  return 0;
}
