#include "main.hpp"
/**
	@file main.cpp
	@brief Entry point of the IRC server program
*/

/**
	@brief Pointer to the global IRC server instance
	This pointer is initialized to NULL and is later assigned a new Server object
	by the main() function. It is used in the sigHandler() function to clean up
	the Server object when the program is terminated by a signal.
*/
Server *g_ircserver = NULL;

void handleUserInput();

/**
	@brief Signal handler for the IRC server program
	This function is registered as the signal handler for the SIGINT signal, which is
	sent to the program when the user presses CTRL-C or the program is terminated
	by the operating system. It checks if the global g_ircserver pointer is not NULL,
	deletes all channels stored in the server's channel list, calls the Server object's
	destructor, and exits the program with a status of 0. If g_ircserver is NULL,
	it simply exits the program with a status of 0.
	@param sig The signal that triggered the signal handler
*/
void sigHandler(int sig)
{
	signal(sig, sigHandler);
	if (g_ircserver != NULL)
		g_ircserver->freeResources();
	delete g_ircserver;
	exit(0);
}

int main(int argc, const char **argv)
{
	if (argc != 3)
	{
		std::cerr << "./ircserv [port number] [password]\n";
		return 1; // Return non-zero to indicate failure
	}

	// Create server object
	Server *server = new Server(argv[1], argv[2]);
	g_ircserver = server;

	// Register signal handler for SIGINT
	signal(SIGINT, sigHandler);

	// Launch server
	server->launchServer();

	// Free resources and delete server object
	server->freeResources();
	delete server;

	return 0; // Return zero to indicate success
}