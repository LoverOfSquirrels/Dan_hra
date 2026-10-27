#include "vila/text.hpp"
#include <algorithm>
#include <string_view>

namespace vila {
namespace {

char32_t nextCodepoint(std::string_view text, std::size_t& position) {
    const auto first = static_cast<unsigned char>(text[position++]);
    if (first < 0x80)
        return first;
    const int count = first >= 0xc2 && first <= 0xdf ? 1
        : first >= 0xe0 && first <= 0xef ? 2 : first >= 0xf0 && first <= 0xf4 ? 3 : 0;
    if (count == 0)
        return U'\ufffd';
    char32_t code = first & (0x7f >> count);
    for (int i = 0; i < count; ++i) {
        if (position == text.size() || (static_cast<unsigned char>(text[position]) & 0xc0) != 0x80)
            return U'\ufffd';
        code = (code << 6) | (static_cast<unsigned char>(text[position++]) & 0x3f);
    }
    const char32_t minimum = count == 1 ? 0x80 : count == 2 ? 0x800 : 0x10000;
    return code < minimum || code > 0x10ffff || (code >= 0xd800 && code <= 0xdfff) ? U'\ufffd' : code;
}

char32_t foldCzech(char32_t code) {
    switch (code) {
    case U'á': case U'Á': return U'a';
    case U'č': case U'Č': return U'c';
    case U'ď': case U'Ď': return U'd';
    case U'é': case U'É': case U'ě': case U'Ě': return U'e';
    case U'í': case U'Í': return U'i';
    case U'ň': case U'Ň': return U'n';
    case U'ó': case U'Ó': return U'o';
    case U'ř': case U'Ř': return U'r';
    case U'š': case U'Š': return U's';
    case U'ť': case U'Ť': return U't';
    case U'ú': case U'Ú': case U'ů': case U'Ů': return U'u';
    case U'ý': case U'Ý': return U'y';
    case U'ž': case U'Ž': return U'z';
    default: return code >= U'A' && code <= U'Z' ? code + (U'a' - U'A') : code;
    }
}

}

std::string Text::normalize(const std::string& input) {
    std::string result;
    bool space = false;
    for (std::size_t position = 0; position < input.size();) {
        const auto start = position;
        const auto code = foldCzech(nextCodepoint(input, position));
        if (code >= 0x0300 && code <= 0x036f)
            continue;
        if (code == U' ' || (code >= 9 && code <= 13) || code == 0xa0) {
            space = !result.empty();
            continue;
        }
        if (space)
            result += ' ';
        space = false;
        if (code < 0x80)
            result += static_cast<char>(code);
        else
            result.append(input, start, position - start);
    }
    return result;
}

bool Text::matches(const std::string& input, const std::string& name,
    const std::vector<std::string>& aliases) {
    return normalize(name) == input || std::any_of(aliases.begin(), aliases.end(),
        [&input](const auto& alias) { return normalize(alias) == input; });
}

std::string Text::join(const std::vector<std::string>& values, const std::string& separator) {
    std::string result;
    for (const auto& value : values) {
        if (!result.empty())
            result += separator;
        result += value;
    }
    return result;
}

}
