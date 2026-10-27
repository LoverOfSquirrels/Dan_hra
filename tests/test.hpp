#pragma once

#include "vila/console.hpp"
#include "vila/text.hpp"
#include "vila/world.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace tests {

using namespace vila;
using Test = std::pair<std::string, std::function<void()>>;

inline void require(bool condition, const std::string& message = "Podmínka neplatí.") {
    if (!condition)
        throw std::runtime_error(message);
}

template<class A, class B>
void equal(const A& expected, const B& actual) {
    require(expected == actual, "Očekávaná a skutečná hodnota se liší.");
}

inline void contains(const std::string& expected, const std::string& actual) {
    require(actual.find(expected) != std::string::npos, "Ve výstupu chybí: " + expected + "\n" + actual);
}

class TestGame {
public:
    GameSession session{WorldFactory::create()};
    GameState& state() { return session.state(); }
    std::string send(const std::string& input) { return dispatcher_.dispatch(parser_.parse(input)); }
    void play(std::initializer_list<std::string> commands) {
        for (const auto& command : commands)
            send(command);
    }
    void repairPower() {
        play({"jdi sever", "jdi vychod", "jdi sever", "vezmi baterii", "jdi jih",
            "jdi vychod", "vezmi drat", "vezmi pacidlo", "spoj drat s baterii",
            "jdi zapad", "jdi zapad", "jdi dolu", "pouzij opravnou sadu na generator"});
    }
    void rescue() {
        play({"pouzij pacidlo na dvere krytu", "jdi sever", "promluv se majitelkou", "odpovez 2"});
    }
    void gatherEvidence() {
        play({"jdi sever", "jdi zapad", "prozkoumej zapisnik", "jdi vychod", "jdi vychod",
            "promluv se spravcem", "odpovez 2", "jdi zapad", "jdi zapad",
            "pouzij klic na dvere pracovny", "jdi sever", "prozkoumej portret", "pouzij 2410 na trezor"});
    }
    void returnFromVeranda() { play({"jdi sever", "jdi zapad", "jdi jih"}); }

private:
    CommandParser parser_;
    CommandDispatcher dispatcher_{session};
};

void addParserTests(std::vector<Test>& tests);
void addWorldTests(std::vector<Test>& tests);
void addStoryTests(std::vector<Test>& tests);
void addPuzzleTests(std::vector<Test>& tests);
void addSaveTests(std::vector<Test>& tests);
void addConsoleTests(std::vector<Test>& tests);

}
