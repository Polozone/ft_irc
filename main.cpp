
#include "main.hpp"

int main(int ac, char **av)
{
	// std::string name1 = "channel1";
	// std::string name2 = "channel2";
	// std::string name3 = "channel3";
	// Channel test;
	// test.addExistingChannels(name1);
	// test.addExistingChannels(name2);
	// test.addExistingChannels(name3);
	// test.printExistingChannels();
	handlerUserInput();
	if (ac != 3)
	{
		std::cerr << "./ircserv [port number] [password]\n";
		return (-1);
	}
	// launchServer(av[1], av[2]);
	return (0);
}
