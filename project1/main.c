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
	char *args[MAX_LINE/2 + 1];  
	char *piped_args[MAX_LINE/2 + 1];
	char user_input[MAX_LINE];
	char history[MAX_LINE];
	int should_run = 1;
	int input;
	int output;
	int pipeline;
	int pipeline_index;

	strcpy(history, "");
	input = 0;
	output = 0;
	pipeline = 0;
	pipeline_index = 0;

	while(should_run){
		printf("osh> ");
		fflush(stdout);

		scanf(" %99[^\n]", user_input);
		fflush(stdin);

		if(!strcmp(user_input, "exit"))  return 0;
	
		if(!strcmp(user_input, "!!")){
			if(!strcmp(history, "")){
				printf("No commands in history\n");
			} else{
				strcpy(user_input, history);
				printf("osh> %s\n", user_input);
			}
		}

		args[0] = strtok(user_input, " ");

		int i = 0;
		int piped_index = 0;
		while(args[i] != NULL){
			if(!strcmp(args[i], "<") && i > 0){
				input = 1;
			} else if(!strcmp(args[i], ">") && i > 0){
				output = 1;
			} else if(!strcmp(args[i], "|") && i > 0){
				pipeline = 1;
				pipeline_index = i;
			}
			i++;		
			args[i] = strtok(NULL, " ");	
		}

		if(pipeline){	
			int k, count;
			for(k = pipeline_index + 1, count = 0; k<i; k++, count++){
				piped_args[count] = args[k];
				args[k] = NULL;
			}
			piped_args[count] = NULL;	

			int pipefd[2];
			
			if(pipe(pipefd) < 0){
				printf("Error on pipe, exiting...\n");
				return -1;
			}  
			
			pid_t pid2 = fork();	
			if(pid2 == 0){ 
				close(pipefd[1]);
				dup2(pipefd[0], 0);
				execvp(piped_args[0], piped_args);
				printf("Piping error has occured\n");
				exit(1);
			} 

			pid_t pid1 = fork();
			if(pid1 == 0){
				close(pipefd[0]);
				dup2(pipefd[1], 1);		
				printf("chekc it %s\n", args[pipeline_index]);
				args[pipeline_index] = NULL;
				execvp(args[0], args);	
				printf("Piping error has occured\n");
				exit(1);
			}	
			
			close(pipefd[0]);
			close(pipefd[1]);
 
			waitpid(pid1, NULL, 0);	
			waitpid(pid2, NULL, 0);

			int j;
			if(strcmp(user_input, "!!")){
				memcpy(history, user_input, MAX_LINE);		
				printf("%s", history);
			}
			for(j=0; j<i; j++){
				args[j] = NULL;
				piped_args[j] = NULL;
			}
		} else{
			pid_t pid = fork();
			if(pid < 0){
				printf("Error on child process creation, exiting...\n");
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
							close(fd);
							execvp(args[0], args);
						} else if(input){
							dup2(fd, 0);
							args[i-1] = NULL;
							args[i-2] = NULL;
							close(fd);
							execvp(args[0], args);
						}
					} else{
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
			strcpy(user_input, "");
			fflush(stdin);
		}
		pipeline = 0;
	}

	return 0;
}
