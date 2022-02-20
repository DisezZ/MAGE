#pragma once

namespace mage
{
    class NoCopy
    {
    public:
        NoCopy(NoCopy const &) = delete;
        NoCopy &operator=(NoCopy const &) = delete;
        NoCopy() = default;
    };
} // namespace mage
