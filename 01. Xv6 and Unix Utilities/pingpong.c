#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int main(int argc, char *argv[]){

	int double_pipe[2]; // Shared Two-way Pipe between the Parent & Child
	pipe(double_pipe);

	char byte[1]; // For Reading from the Pipe

	// Forking
	if (fork()==0){

		// Child Process
		if (read(double_pipe[0], byte, 1)){
			printf("%d: received ping\n", getpid());
			write(double_pipe[1], "p", 1);}
		exit(0);}
		
	else{
		// Parent Process
		write(double_pipe[1], "p", 1);
		wait(0); // Wait for Child to exit successfully
		if (read(double_pipe[0], byte, 1)){
			printf("%d: received pong\n", getpid());}}

	exit(0);
	return 0;}

