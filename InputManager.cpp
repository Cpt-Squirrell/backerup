#include <string>
#include "includes/inputmanager.h"

InputManager::InputManager(int argc, char *arguments[]) {
	//Check if an argument exists
	if (arguments[1] == nullptr)
		option = Options::none;
	else
	{
		//Find the option
		std::string a = arguments[1];
		if (a == "backup")
			option = Options::backup;
		else if (a == "retrieve")
			option = Options::retrieve;
		else if (a == "restore")
			option = Options::restore;
		else if (a == "query")
			option = Options::query;
		else if (a == "list")
			option = Options::list;
		else if (a == "help")
			option = Options::help;
		else if (a == "get")
			option = Options::getConfig;
		else if (a == "set")
			option = Options::setConfig;
	}

	if (option == Options::backup ||
		option == Options::retrieve ||
		option == Options::restore ||
		option == Options::query ||
		option == Options::list)
		argument = arguments[argc - 1]; //TODO: Might be argc (not - 1)
}

InputManager::Options InputManager::getOption() {
	return option;
}

/*InputManager::Flags* InputManager::getFlags()
{
	return flags;
}*/
std::string InputManager::getArgument() {
	return argument;
}