#pragma once

#include "vila/commands.hpp"
#include "vila/services.hpp"

namespace vila {

class CommandDispatcher {
public:
    explicit CommandDispatcher(GameSession& session);
    CommandDispatcher(const CommandDispatcher&) = delete;
    CommandDispatcher& operator=(const CommandDispatcher&) = delete;
    std::string dispatch(const CommandRequest& command);
    static std::string help();

private:
    GameSession& session_;
    GameView view_;
    ObjectFinder finder_;
    NavigationService navigation_;
    ItemService items_;
    PuzzleService puzzles_;
    DialogueService dialogues_;
    ReportService report_;
    HintService hints_;
    std::map<CommandKind, std::function<std::string(const CommandRequest&)>> handlers_;
    void addActions();
    void addQueries();
    static bool allowedAfterEnding(CommandKind kind);
    static bool changesDialogue(CommandKind kind);
};

}
