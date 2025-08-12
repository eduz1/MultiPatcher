#include "ips.h"
#include <stdexcept>

bool IPS::ApplyIpsPatch(std::vector<uint8_t>& file, const std::vector<uint8_t>& ips)
{
    std::vector<uint8_t> patchedFile = file;

    size_t i = 0, ipsSize = ips.size();

	// Check for header
	if (ipsSize < 5 || ips[i++] != 'P' || ips[i++] != 'A' || ips[i++] != 'T' || ips[i++] != 'C' || ips[i++] != 'H')
    {
        errorMsg = "Invalid patch header!";
		return false;
	}

	while (i < ipsSize)
	{
        // offset is 3 bytes
        if (ipsSize < i + 3) 
		{
			errorMsg = "Unexpected end of patch file.";
			return false;
		}

		uint32_t offset = (ips[i++] << 16) | (ips[i++] << 8) | ips[i++];

        // Reading 0x454F46 doubles as the "EOF" ASCII in the patch file.
        // That means a successfully patch, break out and save to output.
		if (offset == 0x454F46)
			break;

        // size is 2 bytes
        if (ipsSize < i + 2)
		{
			errorMsg = "Unexpected end of patch file.";
			return false;
		}

		uint16_t size = (ips[i++] << 8) | ips[i++];

		// RLE encoding indicated by a size = 0
		if (size == 0)
		{
            // RLE encoding needs 3 bytes
            if (ipsSize < i + 3)
			{
				errorMsg = "Unexpected end of patch file.";
				return false;
			}		

			uint16_t rle = (ips[i++] << 8) | ips[i++];
			uint8_t val = ips[i++];

			// If the RLE size is larger than the remaining ROM size, we append it into the rom
			if (patchedFile.size() < offset + rle)
				patchedFile.resize(offset + rle);

			// Write 'val' rle times from offset
			for (int n = 0; n < rle; n++)
				patchedFile[offset + n] = val;
		}
		else
		{
            // Make sure we don't go out of bounds reading data
            if (ipsSize < i + size) 
            {
				errorMsg = "Unexpected end of patch file.";
				return false;
			}

			// If the RLE size is larger than the remaining ROM size, we append it into the rom
			if (patchedFile.size() < offset + size)
				patchedFile.resize(offset + size);

			// Write 'val' size times from offset
			for (int n = 0; n < size; n++)
				patchedFile[offset + n] = ips[i++];
		}
	}

	// Return patched file in place of original rom
	file = patchedFile;

    return true;
}

bool IPS::OutputPatchedFile(const std::vector<uint8_t>& patchedRom, const std::string& outPath)
{
    // Create the output stream
    std::ofstream out(outPath, std::ios::out | std::ios::binary);

    if (out.is_open())
    {
        // Serialize the ROM data and output it
        size_t romSize = patchedRom.size();
        out.write(reinterpret_cast<const char*>(patchedRom.data()), romSize);
        out.close();
    }
    else
        throw std::runtime_error("Error: Unable to save output.");

    return true;
}

// Patches any file with an .ips patch.
// std::string filePath = path to the file to be patched
// std::string ipsPath = path to the patch itself
// (optional) std::string outPath = path for the patched output file
bool IPS::PatchIPS(const std::string& filePath, const std::string& ipsPath, const std::string& outPath)
{
	// Open both IPS and file for read
	std::ifstream inFile(filePath, std::ios::binary);
	std::ifstream inIPS(ipsPath, std::ios::binary);

	if (!inFile)
	{
		errorMsg = "Could not open file!";
		return false;
	}
	if (!inIPS)
	{
		errorMsg = "Could not open patch file!";
		return false;
	}

	// Copy both .ips and the file into vectors for handling
	std::vector<uint8_t> ips{ std::istreambuf_iterator<char>(inIPS), std::istreambuf_iterator<char>() };
	std::vector<uint8_t> file{ std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>() };

	if (!ApplyIpsPatch(file, ips))
	{
		// Already has return msg
		return false;
	}

	std::string outputFilePath;

	if (!outPath.empty()) {
		outputFilePath = outPath;
	} else {
		// Get the extension of the file in a string
		std::string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
		outputFilePath = "output." + fileExtension;
	}

	OutputPatchedFile(file, outputFilePath);

	return true;
}
