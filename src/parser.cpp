#include "vila/commands.hpp"
#include "vila/text.hpp"
#include <map>

namespace vila {
namespace {

const std::map<std::string, CommandKind> simple = {
    {"rozhledni se", CommandKind::Look}, {"rozhlednout se", CommandKind::Look},
    {"rozhledni", CommandKind::Look}, {"inventar", CommandKind::Inventory},
    {"denik", CommandKind::Journal}, {"pomoc", CommandKind::Help},
    {"prikazy", CommandKind::Help}, {"napoveda", CommandKind::Hint},
    {"uloz", CommandKind::Save}, {"nacti", CommandKind::Load},
    {"zprava", CommandKind::Report}, {"odjed", CommandKind::Report}, {"konec", CommandKind::Quit}
};

const std::vector<std::pair<std::string, CommandKind>> verbs = {
    {"jdi na ", CommandKind::Move}, {"jdi do ", CommandKind::Move},
    {"jdi ", CommandKind::Move}, {"vezmi ", CommandKind::Take},
    {"prozkoumej ", CommandKind::Examine}, {"pouzij ", CommandKind::Use},
    {"spoj ", CommandKind::Combine}, {"zkombinuj ", CommandKind::Combine},
    {"promluv se ", CommandKind::Talk}, {"promluv s ", CommandKind::Talk},
    {"odpovez ", CommandKind::Choose}
};

}

CommandRequest CommandParser::parse(const std::string& input) const {
    const auto text = Text::normalize(input);
    const auto found = simple.find(text);
    if (found != simple.end())
        return {found->second};
    for (const auto& [prefix, kind] : verbs)
        if (text.compare(0, prefix.size(), prefix) == 0)
            return parseArguments(kind, text.substr(prefix.size()));
    return {CommandKind::Invalid, text.empty() ? "Zadejte příkaz. Seznam příkazů: pomoc."
        : "Neznámý příkaz. Seznam příkazů: pomoc."};
}

CommandRequest CommandParser::parseArguments(CommandKind kind, const std::string& arguments) {
    if (kind == CommandKind::Use)
        return split(kind, arguments, {" na "}, "použij <předmět> na <cíl>");
    if (kind == CommandKind::Combine)
        return split(kind, arguments, {" s ", " + ", " a "}, "spoj <předmět> s <předmětem>");
    return {kind, arguments};
}

CommandRequest CommandParser::split(CommandKind kind, const std::string& text,
    const std::vector<std::string>& separators, const std::string& syntax) {
    for (const auto& separator : separators) {
        const auto index = text.find(separator);
        if (index != std::string::npos && index > 0 && index + separator.size() < text.size())
            return {kind, text.substr(0, index), text.substr(index + separator.size())};
    }
    return {CommandKind::Invalid, "Použijte: " + syntax + "."};
}

}
