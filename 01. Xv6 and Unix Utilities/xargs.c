#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  // includes MAXARG

int main(int argc, char *argv[]) {
	char buffer[512], *args[MAXARG], letter;
	char *pointer = buffer;
	int c, spaces = 0, index = 0;

	if (argc < 2){
        	printf("Error: Invalid Arguments\n");
	        exit(0);}

	for (c = 0; c < argc-1; c++){
		args[c] = argv[c+1];}

	while (read(0, &letter, 1) > 0){
		if (letter == ' ' || letter == '\t') {
			spaces++;
			continue;}
		
		if (spaces){
			buffer[index] = 0;
			index++;
			args[c] = pointer;
			c++;
			pointer = buffer + index;
			spaces = 0;}

		if (letter != '\n'){
			buffer[index] = letter;
			index++;
		} else {
			args[c] = pointer;
			c++;
			pointer = buffer + index;

			if (fork()==0){
				exit(exec(args[0], args));}
			wait(0);
			c = argc - 1;}}
	exit(0);}

