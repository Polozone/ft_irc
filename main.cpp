
#include "main.hpp"

Server *g_ircserver = NULL;

void handleUserInput();
// Define a function to handle signals
void sigHandler(int sig)
{
    // Ignore the signal
    signal(sig, SIG_IGN);
	std::cout << "value address of g_ircserv in sigHandler >> " << g_ircserver << std::endl;
	g_ircserver->deleteAllChannel();
    // Output a message to the console
    std::cout << "leave by SIGINT" << std::endl;
    // Terminate the program
    exit(0);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "./ircserv [port number] [password]" << std::endl;
		return (-1);
	}
    signal(SIGINT, sigHandler);
	Server(av[1], av[2]);
	std::cout << "value address of g_ircserv in main >> " << g_ircserver << std::endl;
	return (0);
}
// 3831