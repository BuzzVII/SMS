#include "masterSystem.h"
#include "cmdparser.h"
#include <iostream>

int main(int argc, char** argv)
{

    std::vector<std::string> rom;
    bool debug = false;

    CmdTools::CmdParser argParser("MasterSystem [options] rom\n"
                        " return codes:\n"
                        " \t0 Success\n"
                        " \t1 Failure",
                        "-bool",    "%b", &debug,    "debug mode",
                        nullptr);
    
    // parse the command line, and check the result for success
    if (argParser.parse(argc, argv, rom))
    {
        std::cout << "Unknown option used\n";
        argParser.usage();
        return 1;
    }
        
    // if -h was passed in print the usage 
    if (argParser.help())
    {
        argParser.usage();
        return 0;
    }

    
    if (rom.empty())
    {
        std::cout << "Please specify a rom to load\n";
        argParser.usage();
        return 1;
    }
    else if (rom.size() > 1) 
    {
        std::cout << "Ignoring extra rom arguments passed\n";
    }

    const char* cartridge = rom[0].c_str();

    MasterSystem console = MasterSystem::getInstance();

    console.load(cartridge);

    console.powerOn();

    return 0;
}
