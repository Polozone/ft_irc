#include "main.hpp"

Server *g_ircserver = NULL;

void handleUserInput();
// Define a function to handle signals
void sigHandler(int sig)
{
    // Ignore the signal
    signal(sig, SIG_IGN);
    g_ircserver->deleteAllChannel();
    // Output a message to the console
    std::cout << "leave by SIGINT" << std::endl;
    // Terminate the program
    exit(0);
}

int main(int ac, char **av)
{
	// if (ac != 3)
	// {
	// 	std::cerr << "./ircserv [port number] [password]\n";
	// 	return (-1);
	// }
	// handleUserInput();
	//Server test(av[1], "awd");
	g_ircserver = new Server(av[1], av[2]);
    signal(SIGINT, sigHandler);
	delete g_ircserver;
	// Server ircserv(av[1], av[2]);
	return (0);
}
