#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80

int main(void){
	char *args[MAX_LINE/2 + 1];

	int should_run = 1;
	
	fgets(*args, MAX_LINE, stdout);	
	while(should_run){
	//	printf("osh>");
	//	fgets(*args, MAX_LINE, stdout);	
		fflush(stdout);
	}

	return 0;
}
