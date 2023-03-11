#include "main.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "./ircserv [port number]\n";
		return (-1);
	}

	launchServer(av[1]);
	return (0);
}
