#include "main.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "./ircserv [port number] [password]\n";
		return (-1);
	}

	Server ircserv(av[1], av[2]);
	return (0);
}
