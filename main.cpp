#include "main.hpp"

void handleUserInput();

int main(int ac, char **av)
{
	// if (ac != 3)
	// {
	// 	std::cerr << "./ircserv [port number] [password]\n";
	// 	return (-1);
	// }
	// handleUserInput();
	//Server test(av[1], "awd");
	
	Server ircserv(av[1], av[2]);
	return (0);
}
