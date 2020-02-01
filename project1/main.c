#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1]; // this is a string array - this is where we will store every user command 

	int should_run = 1;
	
	fgets(args, MAX_LINE, stdin);
	printf("Input was: %s\n", args);	
	/*while(should_run){
		printf("osh>");
		fgets(*args, MAX_LINE, stdout);	
		fflush(stdout);
	}*/

	return 0;
}
