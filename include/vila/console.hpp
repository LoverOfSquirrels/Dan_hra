#pragma once

#include "vila/dispatcher.hpp"
#include <iosfwd>

namespace vila {

class ConsoleGame {
public:
    ConsoleGame(GameSession& session, std::istream& input, std::ostream& output)
        : session_(session), input_(input), output_(output), dispatcher_(session) {}
    void run();

private:
    GameSession& session_;
    std::istream& input_;
    std::ostream& output_;
    CommandParser parser_;
    CommandDispatcher dispatcher_;
};

}
