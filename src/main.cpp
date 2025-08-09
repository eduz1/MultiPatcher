#include <iostream>
#include "IPS.h"
#include "BPS.h"

int main(int argc, char** argv)
{
    // Check for correct number of arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <patch_file>" << std::endl;
        return -1;
    }

    std::string extension = std::string(argv[2]).substr(std::string(argv[2]).find_last_of(".") + 1);

    // If the extension of the second argument is .ips, use IPS patching
    if (extension == "ips")
    {
         return PatchIPS(argv[1], argv[2]);
    }
    else if (extension == "bps")
    {
         return PatchBPS(argv[1], argv[2]);
    }
    
    return 0;
}