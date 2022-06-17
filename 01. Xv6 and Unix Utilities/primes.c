#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

void next_stage(int left_pipe[]){
	int prime, number; // For iterating over the array of numbers

	int right_pipe[2]; // Initialize the Output Pipe
	pipe(right_pipe);

	close(left_pipe[1]); // Won't write to the Input Pipe

	if (read(left_pipe[0], &prime, sizeof(int))){
		printf("prime: %d\n", prime); // Print the First Number

		if (fork()==0){			
			next_stage(right_pipe);
			exit(0);}
		else{
			close(right_pipe[0]);
			// Remove any Multiples of the current Prime Number
			while (read(left_pipe[0], &number, sizeof(int))){
				if (number % prime != 0){
					write(right_pipe[1], &number, sizeof(int));}}
			close(left_pipe[0]);
			close(right_pipe[1]);
			wait(0);}}	
	exit(0);}

void main(int argc, char *argv[]){
	int p[2];
	pipe(p);

	// Forking
	if (fork()==0){
		// Child Process
		next_stage(p);
		exit(0);
		}
	else{
		// Parent Process
		close(p[0]);
		// Initialize 2~35 Pipe
		for (int i = 2; i<36; i++){
			write(p[1], &i, sizeof(int));}
		close(p[1]);
		wait(0);}
		
	exit(0);}

