                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                d(fd, buffer, init_read) != init_read) {
		printf("%s\n", strerror(errno));
		return -1;
	}
	
	//printf("%s\n", buffer);
	char aux[20];
	strcpy(aux, "#include \"puzzle.h\"");
	write(fd_out, aux, strlen(aux));
	write(fd_out, buffer, init_read);
	
	close(fd);
	return 0;
}

int write_end(int fd_out) {
	int fd = open(file_input, O_RDONLY);
	if (fd == -1) {
		printf("%s\n", strerror(errno));
		return -1;
	}
	
	const int init_read = 282;
	lseek(fd, init_read, SEEK_SET);/* No situamos */


	const int to_read = 3991;
	char buffer[to_read];	

	if (read(fd, buffer, to_read) == -1) {
		printf("%s\n", strerror(errno));
		return -1;
	}
	
	//printf("%s", buffer);
	write(fd_out, buffer, to_read);
	
	close(fd);
	return 0;
}

int main() {
	int numbers[9];
	create_random_puzzle(numbers, 9);/* Creo un estado inicial para el problema del 8 puzzle */
	
	unlink(file_output);
	
	int fd = open(file_output, O_CREAT | O_RDWR, 0777);
	
	if (fd == -1)
		return -1;
	
	write_init(numbers, fd);

	print_8puzzle(numbers, fd);
	
	write_end(fd);
		
	close(fd);
	
	return 0;
}