#pragma once
#include <iostream>

namespace SylvonixNative
{
    struct SoftwareVersion
    {
        uint8_t major;
        uint8_t minor;
        uint8_t patch;

        constexpr SoftwareVersion(
            uint8_t major,
            uint8_t minor,
            uint8_t patch)
            : major(major),
            minor(minor),
            patch(patch)
        {
        }
    };
}