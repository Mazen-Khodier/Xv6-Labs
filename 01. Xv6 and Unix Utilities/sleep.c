#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

	if (argc == 2){
		printf("(nothing happens for a little while)\n");
		sleep(atoi(argv[1]));
		exit(0);}

	else{
		printf("Error: Invalid Arguments\n");
		exit(1);}

	return 0; }
