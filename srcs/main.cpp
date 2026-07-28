#include "server/Server.hpp"
#include "config/Config.hpp"
#include "config/ConfigParser.hpp"
#include <iostream>
#include "colors.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << BOLD_RED << "Usage: " << argv[0] << " <config_file>" << RESET << std::endl;
		return 1;
	}
	try
	{
		ConfigParser configParser;
		std::vector<Config> configs = configParser.parse(argv[1]);
		for (size_t i = 0; i < configs.size(); ++i)
		{
			std::cout << BOLD_GREEN << "Configuration " << i + 1 << ":" << RESET << std::endl;
			configs[i].print();
		}

		Server server(configs);
		server.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << BOLD_RED << "Error: " << e.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}