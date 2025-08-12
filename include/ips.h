#pragma once
#include <vector>
#include <fstream>
#include <string>

class IPS
{
public:
    bool PatchIPS(const std::string& filePath, const std::string& ipsPath, const std::string& outPath = "");
    // Error message will be empty if PatchIps == true.
    std::string errorMsg;

private:
    bool ApplyIpsPatch(std::vector<uint8_t>& file, const std::vector<uint8_t>& ips);
    bool OutputPatchedFile(const std::vector<uint8_t>& patchedFile, const std::string& outPath);
};