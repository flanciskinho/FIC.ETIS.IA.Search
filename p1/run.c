#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define NUM_REPEAT 2

char FILE_OUT[20];

int compile(int cosa) {
	char aux[50];
	strcpy(aux, "./tmp >> ");
	strcat(aux, FILE_OUT);
	if (system(aux) == -1)
		return -1;
	if (system("make clean") == -1)
		return -1;
	if (system("make domain DOM=puzzle") == -1)
		return -1;
	if (system("make") == -1)
		return -1;
		
	return 0;
}

void print_file(const char *aux) {
	char str[512] = {"echo \""};
	strcat(str, aux);
	strcat(str, "\" >> ");
	strcat(str, FILE_OUT);
	
	system(str);
}

void println_file() {
	print_file("\n");	
}

void execute(const char *args) {
	char command[128];
	strcpy(command, "./search ");
	strcat(command, args);
	strcat(command, " >> ");
	strcat(command, FILE_OUT);
	
	print_file(command);
	
	system(command);	
	println_file();
}

void main_execute() {
	char op1 [] = {" -a tree -f depth"};
	char op2 [] = {" -a tree -f breadth"};
	char op3 [] = {" -a tree -f depth"};
	char op4 [] = {" -a tree -f breadth"};
	
	execute(op1);
	execute(op2);
	execute(op3);
	execute(op4);
}

void init() {
	int i;
	struct stat stat;
	for (i = 1; i < 1000; i++) {
		sprintf(FILE_OUT, "out%d.txt", i);
		if (lstat(FILE_OUT, &stat) == -1)
			break; /* No existe el fichero... pues empezar con el */ 
	}
/*
printf("%s\n", FILE_OUT);
char aux[10000];
sprintf(aux, "echo cosa >> %s", FILE_OUT);
system(aux);
exit(0);
*/
	system("gcc -g -Wall tmp.c -o tmp");	
}

int main(void) {
/*
gcc -g -Wall tmp.c -o tmp && ./tmp
make clean
make domain DOM=puzzle
make
./search
*/
	init();
	
	pid_t pid;
	int i;
	char iter [100];
	clock_t t_init, t_end;
	double sgs;

	for (i = 0; i < NUM_REPEAT; i++) {
		pid = fork();
		if (pid == 0) {
			sprintf(iter, "\tIteracion %3d", i);
			print_file(iter);
			if (compile(i) == -1)
				exit(EXIT_FAILURE);
			main_execute();
			exit(EXIT_SUCCESS);
		} else {
			waitpid(pid, NULL, 0);
				
			printf("%d segundos\n", sgs);
		}
	}

}