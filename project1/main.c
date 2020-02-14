#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1]; // this is a string array - this is where we will store every user command 
	char user_input[MAX_LINE];
	char history[MAX_LINE];
	int should_run = 1;
	int input;
	int output;
	
	strcpy(history, "");
	input = 0;
	output = 0;

	while(should_run){
		printf("osh> ");
		fflush(stdout);
		scanf("%[^\n]%*c", user_input);
	
		if(!strcmp(user_input, "exit"))  return 0;
	
		if(!strcmp(user_input, "!!")){
			if(!strcmp(history, "")){
				printf("No commands in history\n");
			}
			else{
				strcpy(user_input, history);
				printf("osh> %s\n", user_input);
			}
		}

		args[0] = strtok(user_input, " ");

		int i = 0;
		while(args[i] != NULL){
			if(!strcmp(args[i], "<") && i > 0){
				input = 1;
			}
			else if(!strcmp(args[i], ">") && i > 0){
				output = 1;
			}
			i++;
			args[i] = strtok(NULL, " ");	
		}
		int j;
		printf("Output is: %d\n", output);	
		pid_t pid = fork();
		if(pid < 0){
			printf("Error on child process creation, exiting...");
			return -1;
		}  
		
		if(pid == 0){
			if(strcmp(user_input, "") && strcmp(user_input, "!!")){
				if(input || output){
					int fd = open(args[i-1], O_RDWR | O_CREAT, S_IROTH | S_IRWXU);
					if(output){
						dup2(fd, 1);
						args[i-1] = NULL;
						args[i-2] = NULL;
						execvp(args[0], args);
					}	
					else if(input){
						dup2(fd, 0);
						args[i-1] = NULL;
						args[i-2] = NULL;
						execvp(args[0], args);
					}
				}
				else{
					execvp(args[0], args);
					printf("Command not found\n");
					exit(1);
				}
			}
			exit(1);
		} else{
			waitpid(pid, NULL, 0);	
			int j;
			input = output = 0;
			if(strcmp(user_input, "!!")){
				strcpy(history, user_input);
			}
			for(j=0; j<i; j++){
				args[j] = NULL;
			}
		}	
	}

	return 0;
}
