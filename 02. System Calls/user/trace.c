#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"
#include "kernel/param.h"  // includes MAXARG

//include "kernel/proc.h"
//#include "user/user.h"
//#include "kernel/memlayout.h"
//#include "kernel/spinlock.h"
//#include "kernel/syscall.h"
//#include "kernel/defs.h"



int main(int argc, char *argv[]) {
	if (argc < 3){
		printf("Error: Invalid Arguments\n");
		exit(1);}
	else{
		int c;
		char *args[MAXARG];
		trace(atoi(argv[1]));
		for (c = 2; c <= argc; c++){
			args[c-2] = argv[c];}
		exec(args[0], args);
		}
	return 0;}

