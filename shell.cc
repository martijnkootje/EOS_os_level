#include "shell.hh"

int main() {
	std::string input;
	std::string prompt;
	int prompt_code = syscall(SYS_open, "Command.txt", O_RDONLY, 0755);
	char byte[1];
	while (syscall(SYS_read, prompt_code, byte, 1)) {
		if (byte[0] == '\n') {
			break;
		}
		prompt.append(byte);
	}
	syscall(SYS_close, prompt_code);

	while (true) {
		std::cout << prompt;
		std::getline(std::cin, input);
		if (input == "new_file")
			new_file();
		else if (input == "ls")
			list();
		else if (input == "src")
			src();
		else if (input == "find")
			find();
		else if (input == "seek")
			seek();
		else if (input == "exit")
			return 0;
		else if (input == "quit")
			return 0;
		else if (input == "error")
			return 1;

		if (std::cin.eof())
			return 0;
	}
}

void new_file() {
	std::cout << "Give a file name: ";
	std::string filename;
	std::cin >> filename;
	std::string input = "";
	int fd = syscall(SYS_open, filename.c_str(), O_CREAT | O_WRONLY, 0755);
	std::string line = "";
	while (std::getline(std::cin, line)) {
		if (input.find("<EOF>") != std::string::npos) {
			break;
		}
		if (line != "") {
			input.append(line + "\n");
		}
	}
	int in = input.length();
	char byte[in];

	strcpy(byte, input.c_str());
	syscall(SYS_write, fd, byte, in - 7);
	syscall(SYS_close, fd);
}

void list() {
	int pid = syscall(SYS_fork);
	if (pid == 0) {
		const char *com[] = { "/bin/ls", "-l", "-a", NULL };
		int fd = syscall(SYS_execve, com[0], com, NULL);
	} else {
		syscall(SYS_wait4, pid, NULL, NULL, NULL);
	}
}

void find() {
	int fd[2];
	syscall(SYS_pipe, &fd, NULL);
	std::string word1;
	std::cout << "Where are you searching for?: ";
	std::cin >> word1;
	int pid = syscall(SYS_fork);
	int pid2;
	if (pid == 0) {
		syscall(SYS_dup2, fd[1], 1);
		syscall(SYS_close, fd[0]);
		const char *com[] = { "/usr/bin/find", ".", NULL };
		syscall(SYS_execve, com[0], com, NULL);
	} else {
		syscall(SYS_wait4, pid, NULL, NULL, NULL);
		pid2 = syscall(SYS_fork);
		if (pid2 == 0) {
			syscall(SYS_close, fd[1]);
			syscall(SYS_dup2, fd[0], 0);
			const char *com[] = { "/bin/grep", word1.c_str(), NULL };
			syscall(SYS_execve, com[0], com, NULL);
		}
		syscall(SYS_close, fd[0]);
		syscall(SYS_close, fd[1]);
		syscall(SYS_wait4, pid2, NULL, NULL, NULL);
		std::cin.ignore(1);
	}
}

void seek() {
	std::string seek = "seek";
	std::string loop = "loop";
	int seek_file = syscall(SYS_creat, seek.c_str(), 0644);
	int loop_file = syscall(SYS_creat, loop.c_str(), 0644);
	const char *x = "x";
	const char *s = "\0";
	int pid = syscall(SYS_fork);
	if (pid == 0) {
		syscall(SYS_write, seek_file, x, 1);
		int pos = syscall(SYS_lseek, seek_file, 5000000, SEEK_END);
		syscall(SYS_write, seek_file, x, 1);
		syscall(SYS_close, seek_file);
	} else {
		syscall(SYS_write, loop_file, x, 1);
		for (int i = 0; i < 5000000; i++) {
			syscall(SYS_write, loop_file, s, 1);
		}
		syscall(SYS_write, loop_file, x, 1);
		syscall(SYS_close, loop_file);
		syscall(SYS_wait4, pid, NULL, NULL, NULL);
	}
}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{
	int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
	char byte[1]; // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
	while (syscall(SYS_read, fd, byte, 1)) // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
		std::cout << byte;
}           //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.

