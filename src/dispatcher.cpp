#include "vila/dispatcher.hpp"

namespace vila {

CommandDispatcher::CommandDispatcher(GameSession& session)
    : session_(session), view_(session), finder_(session), navigation_(session, view_),
      items_(session, finder_), puzzles_(session, finder_), dialogues_(session, finder_, view_),
      report_(session), hints_(session) {
    addActions();
    addQueries();
}

std::string CommandDispatcher::dispatch(const CommandRequest& command) {
    if (session_.state().ending && !allowedAfterEnding(command.kind))
        return "Vyšetřování je uzavřené. Můžete načíst uloženou hru nebo zadat konec.";
    if (changesDialogue(command.kind))
        session_.state().activeCharacter.reset();
    return handlers_.at(command.kind)(command);
}

void CommandDispatcher::addActions() {
    handlers_[CommandKind::Move] = [this](const auto& c) { return navigation_.move(c.subject); };
    handlers_[CommandKind::Take] = [this](const auto& c) { return items_.take(c.subject); };
    handlers_[CommandKind::Examine] = [this](const auto& c) { return items_.examine(c.subject); };
    handlers_[CommandKind::Use] = [this](const auto& c) { return puzzles_.use(c.subject, c.target); };
    handlers_[CommandKind::Combine] = [this](const auto& c) { return items_.combine(c.subject, c.target); };
    handlers_[CommandKind::Talk] = [this](const auto& c) { return dialogues_.talk(c.subject); };
    handlers_[CommandKind::Choose] = [this](const auto& c) { return dialogues_.choose(c.subject); };
    handlers_[CommandKind::Report] = [this](const auto&) { return report_.finish(); };
}

void CommandDispatcher::addQueries() {
    handlers_[CommandKind::Look] = [this](const auto&) { return view_.look(); };
    handlers_[CommandKind::Inventory] = [this](const auto&) { return view_.inventory(); };
    handlers_[CommandKind::Journal] = [this](const auto&) { return view_.journal(); };
    handlers_[CommandKind::Help] = [](const auto&) { return help(); };
    handlers_[CommandKind::Hint] = [this](const auto&) { return hints_.hint(); };
    handlers_[CommandKind::Save] = [this](const auto&) { return session_.save(); };
    handlers_[CommandKind::Load] = [this](const auto&) { return session_.load() + "\n" + view_.look(); };
    handlers_[CommandKind::Quit] = [](const auto&) { return "Aplikace ukončena."; };
    handlers_[CommandKind::Invalid] = [](const auto& c) { return c.subject; };
}

bool CommandDispatcher::allowedAfterEnding(CommandKind kind) {
    static const std::set<CommandKind> allowed{CommandKind::Look, CommandKind::Inventory, CommandKind::Journal,
        CommandKind::Help, CommandKind::Save, CommandKind::Load, CommandKind::Quit, CommandKind::Invalid};
    return allowed.count(kind) != 0;
}

bool CommandDispatcher::changesDialogue(CommandKind kind) {
    static const std::set<CommandKind> changes{CommandKind::Move, CommandKind::Take, CommandKind::Examine,
        CommandKind::Use, CommandKind::Combine, CommandKind::Report, CommandKind::Talk};
    return changes.count(kind) != 0;
}

std::string CommandDispatcher::help() {
    return "rozhlédni se\n"
        "jdi <směr nebo název východu>\n"
        "vezmi <předmět>\n"
        "prozkoumej <předmět nebo objekt>\n"
        "použij <předmět nebo kód> na <cíl>\n"
        "spoj <předmět> s <předmětem>\n"
        "promluv se <postavou>\n"
        "odpověz <číslo>\n"
        "inventář | deník | nápověda | pomoc\n"
        "ulož | načti\n"
        "zpráva (u auta uzavře vyšetřování)\n"
        "konec (ukončí aplikaci)\n"
        "Příkazy fungují také bez diakritiky.";
}

}
