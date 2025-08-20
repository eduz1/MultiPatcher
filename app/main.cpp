#include <iostream>
#include "IPS.h"

int main(int argc, char** argv) {
   // Check for correct number of arguments
   if (argc <= 2 || 5 >= argc) {
      std::cerr << "Usage: " << argv[0] << " <input_file> <patch_file> [output_file (optional)]" << std::endl;
      return -1;
   }

   std::string extension = std::string(argv[2]).substr(std::string(argv[2]).find_last_of(".") + 1);

   // If the extension of the second argument is .ips, use IPS patching
   if (extension == "ips") {
      IPS ipsPatcher;

      if (argc == 4) {
         if (ipsPatcher.PatchIPS(argv[1], argv[2], argv[3])) {
            std::cout << "Patch applied successfully." << std::endl;
         }
         else {
            std::cerr << "Error: " << ipsPatcher.errorMsg << std::endl;
            return -1;
         }
      }
      else {
         if (ipsPatcher.PatchIPS(argv[1], argv[2])) {
            std::cout << "Patch applied successfully." << std::endl;
         }
         else {
            std::cerr << "Error: " << ipsPatcher.errorMsg << std::endl;
            return -1;
         }
      }
   }

   return 0;
}