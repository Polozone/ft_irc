#include "main.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "./ircserv [port number] [password]\n";
		return (-1);
	}

	launchServer(av[1], av[2]);
	return (0);
}
