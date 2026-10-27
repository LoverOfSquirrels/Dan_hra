#pragma once

#include <string>
#include <vector>

namespace vila {

class Text {
public:
    static std::string normalize(const std::string& input);
    static bool matches(const std::string& input, const std::string& name,
        const std::vector<std::string>& aliases);
    static std::string join(const std::vector<std::string>& values, const std::string& separator);
};

}
