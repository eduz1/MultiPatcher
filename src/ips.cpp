#include "ips.h"
#include <stdexcept>

// Patches any file with an .ips patch.
// std::string filePath = path to the file to be patched
// std::string ipsPath = path to the patch itself
// (optional) std::string outPath = path for the patched output file
bool PatchIPS(const std::string& filePath, const std::string& ipsPath)
{
	// Open both IPS and file for read
	std::ifstream inFile(filePath, std::ios::binary);
	std::ifstream inIPS(ipsPath, std::ios::binary);
	
	if (!inFile)
		throw std::invalid_argument("Could not open file!");

	if (!inIPS)
 		throw std::invalid_argument("Could not open patch file!");

	// Copy both .ips and the file into vectors for handling
	std::vector<uint8_t> ips{ std::istreambuf_iterator<char>(inIPS), std::istreambuf_iterator<char>() };
	std::vector<uint8_t> rom{ std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>() };
	uint32_t i = 0, ipsSize = static_cast<uint32_t>(ips.size());
	inIPS.close();
	inFile.close();

	// Check for header
	if (ipsSize < 5 || ips[i++] != 'P' || ips[i++] != 'A' || ips[i++] != 'T' || ips[i++] != 'C' || ips[i++] != 'H')
        throw std::runtime_error("Invalid patch header!");


	while (i < ipsSize)
	{
        // offset is 3 bytes
        if (ipsSize < i + 3) 
            throw std::runtime_error("Unexpected end of patch file.");

		uint32_t offset = (ips[i++] << 16) | (ips[i++] << 8) | ips[i++];

        // Reading 0x454F46 doubles as the "EOF" ASCII in the patch file.
        // That means a successfully patch, break out and save to output.
		if (offset == 0x454F46)
		{
			break;
		}

        // size is 2 bytes
        if (ipsSize < i + 2) 
            throw std::runtime_error("Unexpected end of patch file.");

		uint16_t size = (ips[i++] << 8) | ips[i++];

		if (!size)
		{
            // RLE encoding needs 3 bytes
            if (ipsSize < i + 3) 
                throw std::runtime_error("Unexpected end of patch file.");

			uint16_t rle = (ips[i++] << 8) | ips[i++];
			uint8_t val = ips[i++];

			// If the RLE size is larger than the remaining ROM size, we append it into the rom
			if (rom.size() < offset + rle)
				rom.resize(offset + rle);

			// Write 'val' rle times from offset
			for (int n = 0; n < rle; n++)
				rom[offset + n] = val;
		}
		else
		{
            // Make sure we don't go out of bounds reading data
            if (ipsSize < i + size) 
                throw std::runtime_error("Unexpected end of patch file.");

			// If the RLE size is larger than the remaining ROM size, we append it into the rom
			if (rom.size() < offset + size)
				rom.resize(offset + size);

			// Write 'val' size times from offset
			for (int n = 0; n < size; n++)
				rom[offset + n] = ips[i++];
		}
	}

    //Get the extension of the file in a string
    std::string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);

	// Create the output stream
	std::ofstream out("output." + fileExtension, std::ios::out | std::ios::binary);

	if (out.is_open())
	{
        // Serialize the ROM data and output it
		size_t romSize = rom.size();
		out.write(reinterpret_cast<const char*>(rom.data()), romSize);
		out.close();
	}
	else
	{
        throw std::runtime_error("Error: Unable to save output.");
	}

	return true;
}
