#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1]; // this is a string array - this is where we will store every user command 
	char user_input[MAX_LINE];
	char *history;
	int should_run = 1;
	
	while(should_run){
		printf("osh> ");
		fflush(stdout);
		scanf ("%[^\n]%*c", user_input);
	
		if(!strcmp(user_input, "exit"))  return 0;
		
		args[0] = strtok(user_input, " ");

		int i = 0;
		while(args[i] != NULL){
			i++;
			args[i] = strtok(NULL, " ");
		}
		int j;
				
		pid_t pid = fork();
		if(pid < 0){
			printf("Error on child process creation, exiting...");
			return -1;
		}  
		
		if(pid == 0){
			execvp(args[0], args);
			printf("Command not found\n");
			exit(1);	
		} else{
			waitpid(pid, NULL, 0);	
			int j;
			history = malloc(sizeof user_input);
			for(j=0; j<i; j++){
				args[j] = NULL;
			}
		}	
	}

	return 0;
}
