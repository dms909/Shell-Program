#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1]; // this is a string array - this is where we will store every user command 
	char user_input[MAX_LINE];
	int should_run = 1;
	
	while(should_run){
		printf("osh>");
		fflush(stdout);

		scanf("%s", user_input);

		if(!strcmp(user_input, "exit"))  return 0;

		*args = strtok(user_input, " ");
		printf("%s", args[1]);
		/*pid_t pid = fork();  
		if(!pid){
			printf("This is the child process");
		} else{
			printf("This is the parent process");
		}
*/
		printf("\n");
	}

	return 0;
}
