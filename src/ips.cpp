#include "ips.h"
#include <filesystem>

bool IPS::ApplyIPSPatch(std::vector<uint8_t>& file, const std::vector<uint8_t>& ips)
{
	size_t i = 0, ipsSize = ips.size();

	// Check for header
	if (ipsSize < 5 || ips[i++] != 'P' || ips[i++] != 'A' || ips[i++] != 'T' || ips[i++] != 'C' || ips[i++] != 'H') {
		errorMsg = "Invalid patch header!";
		return false;
	}

	while (i < ipsSize) {
		// offset is 3 bytes
		if (ipsSize < i + 3) {
			errorMsg = "Unexpected end of patch file.";
			return false;
		}

		uint32_t offset = (ips[i++] << 16) | (ips[i++] << 8) | ips[i++];

		// Reading 0x454F46 doubles as the "EOF" ASCII in the patch file.
		// That means a successfully patch, break out and save to output.
		if (offset == 0x454F46)
			break;

		// size is 2 bytes
		if (ipsSize < i + 2) {
			errorMsg = "Unexpected end of patch file.";
			return false;
		}

		uint16_t size = (ips[i++] << 8) | ips[i++];

		// RLE encoding indicated by a size = 0
		if (size == 0) {
			// RLE encoding needs 3 bytes
			if (ipsSize < i + 3) {
				errorMsg = "Unexpected end of patch file.";
				return false;
			}

			uint16_t rle = (ips[i++] << 8) | ips[i++];
			uint8_t val = ips[i++];

			// If the RLE size is larger than the remaining file size, we append it into the file
			if (file.size() < offset + rle)
				file.resize(offset + rle);

			// Write 'val' rle times from offset
			std::fill_n(file.begin() + offset, rle, val);
		}
		else {
			// Make sure we don't go out of bounds reading data
			if (ipsSize < i + size) {
				errorMsg = "Unexpected end of patch file.";
				return false;
			}

			// If the RLE size is larger than the remaining file size, we append it into the file
			if (file.size() < offset + size)
				file.resize(offset + size);

			// Write 'val' size times from offset
			std::copy_n(ips.begin() + i, size, file.begin() + offset);
			i += size;
		}
	}

	return true;
}

bool IPS::OutputPatchedFile(const std::vector<uint8_t>& patchedFile, const std::string& outPath) {
	// Create the output stream
	std::ofstream out(outPath, std::ios::out | std::ios::binary);

	if (out.is_open()) {
		// Serialize the file data and output it
		size_t fileSize = patchedFile.size();
		out.write(reinterpret_cast<const char*>(patchedFile.data()), fileSize);

		// Check for write errors
		if (out.fail()) {
			out.close();
			throw std::runtime_error("Error: Failed to write output file.");
		}

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
bool IPS::PatchIPS(const std::string& filePath, const std::string& ipsPath, const std::string& outPath) {
	// Open both IPS and file for read
	std::ifstream inFile(filePath, std::ios::binary);
	std::ifstream inIPS(ipsPath, std::ios::binary);

	if (!inFile) {
		errorMsg = "Could not open file!";
		return false;
	}
	if (!inIPS) {
		errorMsg = "Could not open patch file!";
		return false;
	}

	// Copy both .ips and the file into vectors for handling
	std::vector<uint8_t> file{ std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>() };
	std::vector<uint8_t> ips{ std::istreambuf_iterator<char>(inIPS), std::istreambuf_iterator<char>() };

	if (!ApplyIPSPatch(file, ips)) {
		// Already has return msg
		return false;
	}

	std::string outputFilePath;

	if (!outPath.empty()) {
		outputFilePath = outPath;
	}
	else {
		// Get the extension of the file in a string
		std::string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
		outputFilePath = "output." + fileExtension;
	}

	OutputPatchedFile(file, outputFilePath);

	return true;
}