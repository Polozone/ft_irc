#include "main.hpp"

void handleUserInput();

void  sigHandler(int sig)
{
	signal(sig, SIG_IGN);
	std::cout << "leave by SIGINT" << std::endl;
	exit(0);
}

int main(int ac, char **av)
{
	signal(SIGINT, sigHandler);
	Server ircserv(av[1], av[2]);
	return (0);
}
