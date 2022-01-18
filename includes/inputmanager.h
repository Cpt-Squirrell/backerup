#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include <string>
#include <filesystem>
class InputManager
{
    public:
        enum Options
        {
            backup,
            retrieve,
            setConfig,
            getConfig
        };
        enum Flags
		{

		};
        InputManager(int argc, char *arguments[]);
        Options getOption();
        Flags* getFlags();
        std::string getArgument();
    
    private:
        std::string argument;
        Options option;
        //Flags flags[];
};
#endif // __INPUTMANAGER_H__