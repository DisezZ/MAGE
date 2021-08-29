#pragma once

#include "slx.hpp"
#include "NoCopy.hpp"

namespace mage
{
    class MagePipeline final : NoCopy
    {
    private:
        static std::vector<char> readFile(const std::string &filePath);
        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath);
    public:
        MagePipeline(const std::string &vertFilePath, const std::string &fragFilePath);
        ~MagePipeline();
    };
} // namespace mage