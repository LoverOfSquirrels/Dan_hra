#pragma once

#include <string>
#include <vector>

namespace vila {

enum class CommandKind {
    Look, Move, Take, Examine, Use, Combine, Talk, Choose,
    Inventory, Journal, Help, Hint, Save, Load, Report, Quit, Invalid
};

struct CommandRequest {
    CommandKind kind;
    std::string subject = "";
    std::string target = "";
};

class CommandParser {
public:
    CommandRequest parse(const std::string& input) const;

private:
    static CommandRequest parseArguments(CommandKind kind, const std::string& arguments);
    static CommandRequest split(CommandKind kind, const std::string& text,
        const std::vector<std::string>& separators, const std::string& syntax);
};

}
